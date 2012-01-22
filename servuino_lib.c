/*  Servuino is a Arduino Simulator Engine
    Copyright (C) 2012  Benny Saxen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/
//====================================
void clearRW()
//====================================
{
  int i;
  for(i=0;i<g_nTotPins;i++)
    {
      x_pinRW[i] = 0;
    }
}
//====================================
void interruptPinValues()
//====================================
{
  int i;
  for(i=0;i<g_nTotPins;i++)
    {
      if(g_attachedPin[i] == YES)
	{
	  x_pinDigValue[i] = x_pinScenario[i][g_curStep];
	  x_pinMode[i]     = g_interruptType[i];
	}
    }
}
//====================================
void logEvent(char ev[])
//====================================
{
  fprintf(f_event,"+ %d ? %s\n",g_curStep,ev);
}

//====================================
void logCust(char ev[])
//====================================
{
  if(strstr(ev,"no-text") == NULL)
    fprintf(f_cust,"%d %s\n",g_curStep,ev);
}

//====================================
void logStatus()
//====================================
{
  //fprintf(s_log,"%d %s\n",g_curStep,ev);
}

//====================================
int servuinoFunc(int event, int pin, int value, const char *p)
//====================================
{
  int res=999,fail=0;
  char eventText[120];
  char custText[120];

  strcpy(custText,"no-text");

  g_curStep++;

  updateFromRegister();
  clearRW();
  interruptPinValues();
  if(g_serialMode == ON)
    {
      x_pinMode[0] = RX;
      x_pinMode[1] = TX;
    }
  
  sprintf(eventText,"%d Unknown event: %d pin=%d value=%d",g_curStep,event,pin,value);
  
  if(event == S_UNIMPLEMENTED)
    {
      sprintf(eventText,"Unimplemented: %s",p);
    }
  if(event == S_SETUP)
    {
      sprintf(eventText,"setup");
    }
  if(event == S_LOOP)
    {
      sprintf(eventText,"servuinoLoop %d",g_curLoop);
    }
  if(event == S_PIN_MODE_INPUT)
    {
      fail = checkRange(FAIL,"digpin",pin);
      if(fail == 0)
	{
	  x_pinMode[pin] = value;
	  if(value==INPUT)
	    {
	      writeRegister(1,R_DDR,pin,0);
	      sprintf(eventText,"pinMode pin=%d INPUT",pin);
	      sprintf(custText,"%s %d",g_custText[event][pin],pin);
	    }
	}
      res = 0;
    }
  if(event == S_PIN_MODE_OUTPUT)
    {
      fail = checkRange(FAIL,"digpin",pin);
      if(fail == 0)
	{
	  x_pinMode[pin] = value;
	  if(value==OUTPUT)
	    {
	      writeRegister(1,R_DDR,pin,1);
	      sprintf(eventText,"pinMode pin=%d OUTPUT",pin);
	      sprintf(custText,"%s %d",g_custText[event][pin],pin);
	    }
	}
      res = 0;
    }
  if(event == S_DIGITAL_WRITE_LOW || event == S_DIGITAL_WRITE_HIGH)
    {
      x_pinRW[pin] = T_WRITE;
      fail = checkRange(FAIL,"digpin",pin);
      fail = fail + checkRange(FAIL,"digval",value);
      if(fail == 0)
	{
	  x_pinDigValue[pin] = value;
	  writeRegister(1,R_PORT,pin,value);
	  sprintf(eventText,"digitalWrite pin=%d value=%d",pin,value);
	  sprintf(custText,"%s %d",g_custText[event][pin],value);
	}
      res = 0;
      if(x_pinMode[pin] != OUTPUT) 
	errorLog("DigitalWrite when pin Mode is INPUT",pin);
    }
  if(event == S_DIGITAL_READ)
    {
      x_pinRW[pin] = T_READ;
      res = x_pinScenario[pin][g_curStep];
      x_pinDigValue[pin] = res;
      sprintf(eventText,"digitalRead pin=%d value=%d",pin,res);
      sprintf(custText,"%s %d",g_custText[event][pin],res);
      //value = getDigitalPinValue(pin,currentStep); 
      if(x_pinMode[pin] != INPUT) 
	errorLog("DigitalRead when pin Mode is OUPUT",pin); 
    }
  if(event == S_ANALOG_WRITE)//PWM
    {
      x_pinRW[pin] = T_WRITE;
      fail = checkRange(FAIL,"pwmpin",pin);
      fail = fail + checkRange(FAIL,"pwmval",value);
      if(fail == 0)
	{
	  x_pinAnaValue[pin] = value;
	  //writeRegister(pin,value);
	  sprintf(eventText,"analogWrite pin=%d value=%d",pin,value);
	  sprintf(custText,"%s %d",g_custText[event][pin],value);
	}
      res = 0;
    }
  if(event == S_ANALOG_READ)
    {
      x_pinRW[pin] = T_READ;
      res = x_pinScenario[pin][g_curStep];
      x_pinAnaValue[pin] = res;
      sprintf(eventText,"analogRead pin=%d value=%d",pin-g_nDigPins,res);
      sprintf(custText,"%s %d",g_custText[event][pin],res);
    }
  if(event == S_ANALOG_REFERENCE)
    {
      sprintf(eventText,"analogReference type=%s",p);
    }
  if(event == S_DELAY)
    {
      sprintf(eventText,"delay %d ms",pin);
      fprintf(f_time,"+ %d %d\n",g_curStep,pin*1000);
    }
  if(event == S_DELAY_MS)
    {
      sprintf(eventText,"delay %d us",pin);
      fprintf(f_time,"+ %d %d\n",g_curStep,pin);
    }
  if(event == S_ATTACH_INTERRUPT_LOW)
    {
      sprintf(eventText,"attached LOW interrupt %d",pin);
    }
  if(event == S_ATTACH_INTERRUPT_RISING)
    {
      sprintf(eventText,"attached RISING interrupt %d",pin);
    }
  if(event == S_ATTACH_INTERRUPT_FALLING)
    {
      sprintf(eventText,"attached FALLING interrupt %d",pin);
    }
  if(event == S_ATTACH_INTERRUPT_CHANGE)
    {
      sprintf(eventText,"attached CHANGE interrupt %d",pin);
    }
  if(event == S_DETACH_INTERRUPT)
    {
      g_interruptType[pin] = 0;
      sprintf(eventText,"detached interrupt %d",pin);
    }
  if(event == S_SERIAL_BEGIN)
    {
      sprintf(eventText,"Serial.begin baud=%d",pin);
      if(g_serialMode == ON) 
	errorLog("Serial begin already exists",g_curStep);
      g_serialMode = ON;
    }
  if(event == S_SERIAL_END)
    {
      sprintf(eventText,"Serial.end");
      if(g_serialMode == ON) 
	errorLog("Serial end without serial.begin",g_curStep);
      g_serialMode = OFF;
    }
  if(event == S_SERIAL_PRINT_INT)
    {
      sprintf(eventText,"Serial.print(int) %d",pin);
      fprintf(f_serial,"%d SL [%d]\n",g_curStep,pin);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }
  if(event == S_SERIAL_PRINT_INT_BASE)
    {
      sprintf(eventText,"Serial.print(int,base) %d base=%d",pin,value);
      fprintf(f_serial,"%d SL [%d]\n",g_curStep,pin);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }
  if(event == S_SERIAL_PRINT_CHAR)
    {
      sprintf(eventText,"Serial.print(char) %s",p);
      fprintf(f_serial,"%d SL [%s]\n",g_curStep,p);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }
  if(event == S_SERIAL_PRINT_STRING)
    {
      sprintf(eventText,"Serial.print(string) %s",p); 
      fprintf(f_serial,"%d SL [%s]\n",g_curStep,p);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }
  if(event == S_SERIAL_PRINT_SSTRING)
    {
      sprintf(eventText,"Serial.print(String) %s",p);
      fprintf(f_serial,"%d SL [%s]\n",g_curStep,p);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }
  if(event == S_SERIAL_PRINTLN_INT)
    { 
      sprintf(eventText,"Serial.println(int) %d",pin);
      fprintf(f_serial,"%d NL [%d]\n",g_curStep,pin);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }
  if(event == S_SERIAL_PRINTLN_INT_BASE)
    {
      sprintf(eventText,"Serial.println(int,base) %d base=%d",pin,value);
      fprintf(f_serial,"%d NL [%d]\n",g_curStep,pin);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }
  if(event == S_SERIAL_PRINTLN_CHAR)
    { 
      sprintf(eventText,"Serial.println(char) %s",p);
      fprintf(f_serial,"%d NL [%s]\n",g_curStep,p);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }
  if(event == S_SERIAL_PRINTLN_STRING)
    {
      sprintf(eventText,"Serial.println(string) %s",p); 
      fprintf(f_serial,"%d NL [%s]\n",g_curStep,p);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }
  if(event == S_SERIAL_PRINTLN_SSTRING)
    {
      sprintf(eventText,"Serial.println(String) %s",p);
      fprintf(f_serial,"%d NL [%s]\n",g_curStep,p);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }
  if(event == S_SERIAL_PRINTLN_VOID)
    {
      sprintf(eventText,"Serial.println(void)");
      fprintf(f_serial,"%d NL []\n",g_curStep);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }
  if(event == S_SERIAL_WRITE)
    {
      sprintf(eventText,"Serial.write %s",p);
      if(g_serialMode != ON) 
	errorLog("Serial print without serial.begin",g_curStep);
    }

  logEvent(eventText);
  logCust(custText);
  writeStatus();
  registerLog();

  if(g_curStep == g_simulationLength) stopEncoding();

  interruptNow();
  return(res);
}
//====================================
void writeRegister(int digital, int reg, int port, int value)
//====================================
{
  //printf("digital=%d reg=%d pin=%d value=%d\n",digital,reg,port,value);

  if(g_boardType == UNO)
    {
      //-------------------------------------------------------
      if(reg == R_PORT && digital == 1) // 0=LOW 1=HIGH
	{
	  if(port <= 7 && port >=  0)bitWrite(&PORTD,port,value);
	  if(port >= 8 && port <= 13)bitWrite(&PORTB,port-8,value);
	}
      if(reg == R_PORT && digital == 0)
	{
	  if(port <= 5 && port >=  0)bitWrite(&PORTC,port,value);
	}
      //-------------------------------------------------------
      if(reg == R_DDR && digital == 1)  // 0=INPUT 1=OUTPUT
	{
	  if(port <= 7 && port >=  0)bitWrite(&DDRD,port,value);
	  if(port >= 8 && port <= 13)bitWrite(&DDRB,port-8,value);
	}
      if(reg == R_DDR && digital == 0)
	{
	  if(port <= 5 && port >=  0)bitWrite(&DDRC,port,value);
	}
      //-------------------------------------------------------
      if(reg == R_PIN && digital == 1)  // 0=INPUT 1=OUTPUT
	{
	  if(port <= 7 && port >=  0)bitWrite(&PIND,port,value);
	  if(port >= 8 && port <= 13)bitWrite(&PINB,port-8,value);
	}
      if(reg == R_PIN && digital == 0)
	{
	  if(port <= 5 && port >=  0)bitWrite(&PINC,port,value);
	}
      //-------------------------------------------------------
    }
  else
    errorLog("writeRegister: Not UNO board",g_curStep);
}

//====================================
int readRegister(int reg, int port)
//====================================
{
  int value = 99;

  if(g_boardType == UNO)
    {
      //-------------------------------------------------------
      if(reg == R_PORT && port < g_nDigPins) // 0=LOW 1=HIGH
	{
	  if(port <= 7 && port >=  0)value=bitRead(PORTD,port);
	  if(port >= 8 && port <= 13)value=bitRead(PORTB,port-8);
	}
      if(reg == R_PORT && port >= g_nDigPins) // Analog Pins
	{
	  if(port-g_nDigPins <= 5 && port-g_nDigPins >=  0)value=bitRead(PORTC,port);
	}
      //-------------------------------------------------------
      if(reg == R_DDR && port < g_nDigPins)  // 0=INPUT 1=OUTPUT
	{
	  if(port <= 7 && port >=  0)value=bitRead(DDRD,port);
	  if(port >= 8 && port <= 13)value=bitRead(DDRB,port-8);
	}
      if(reg == R_DDR && port >= g_nDigPins)// Analog Pins
	{
	  if(port-g_nDigPins <= 5 && port-g_nDigPins >=  0)value=bitRead(DDRC,port);
	}
      //-------------------------------------------------------
      if(reg == R_PIN && port < g_nDigPins)  // 0=INPUT 1=OUTPUT
	{
	  if(port <= 7 && port >=  0)value=bitRead(PIND,port);
	  if(port >= 8 && port <= 13)value=bitRead(PINB,port-8);
	}
      if(reg == R_PIN && port >= g_nDigPins)// Analog Pins
	{
	  if(port-g_nDigPins <= 5 && port-g_nDigPins >=  0)value=bitRead(PINC,port);
	}
      //-------------------------------------------------------
    }
  else
    errorLog("readRegister: Not UNO board",g_curStep);
  
  return(value);
}

//====================================
void updateFromRegister()
//====================================
{
  int i;

  for(i=0;i<g_nTotPins;i++)
    {      
      // Pin Mode
      x_pinMode[i] = readRegister(R_DDR,i);
      // Pin Value Output
      x_pinDigValue[i] = readRegister(R_PORT,i);

      // Pin Value Input - do not update from register !!
    }


}
//====================================
void writeStatus()
//====================================
{
  int i;
  
  fprintf(f_pinmod,"+ %d ? ",g_curStep);
  for(i=0;i<g_nTotPins;i++)
    {
      fprintf(f_pinmod,"%d,",x_pinMode[i]);
    }
  fprintf(f_pinmod,"\n");
  
  fprintf(f_digval,"+ %d ? ",g_curStep);
  for(i=0;i<g_nTotPins;i++)
    {
      fprintf(f_digval,"%d,",x_pinDigValue[i]);
    }
  fprintf(f_digval,"\n");
  
  fprintf(f_anaval,"+ %d ? ",g_curStep);
  for(i=0;i<g_nTotPins;i++)
    {
      fprintf(f_anaval,"%d,",x_pinAnaValue[i]);
    }
  fprintf(f_anaval,"\n");
  
  fprintf(f_pinrw,"+ %d ? ",g_curStep);
  for(i=0;i<g_nTotPins;i++)
    {
      fprintf(f_pinrw,"%d,",x_pinRW[i]);
    }
  fprintf(f_pinrw,"\n");
  
}

//====================================
void statusLog()
//====================================
{
  int x,n,i,in_out;
  char z[200];
  int tempA[MAX_PIN_ANALOG_MEGA];
  int tempD[MAX_PIN_DIGITAL_MEGA];
  int tempM[MAX_PIN_DIGITAL_MEGA];
  int pinA[MAX_PIN_ANALOG_MEGA];
  int pinD[MAX_PIN_DIGITAL_MEGA];
  int pinM[MAX_PIN_DIGITAL_MEGA];
  

  tempA[0] = 0;
  tempD[0] = 0;
  for(i=0;i<=max_anaPin;i++)
    {
      tempA[i] = 0;
      pinA[i]  = 0; 
    }
  for(i=0;i<=max_digPin;i++)
    {
      tempD[i] = 0;
      pinD[i]  = 0; 
    }

  // step,status dig pins, dig pin value, n ana pair, n PWM dig pair, ( pairs)

  // Value status of analog pins
  n = 0;
  for(i=0;i<=max_anaPin;i++)
    {
      x = c_analogPin[i];
      if(x != 0)
	{
	  n++;
	  tempA[0] = n;
	  tempA[n] = x;
	  pinA[n]  = i;
	}  
    }
  

  // Value status of digital pins
  n = 0;
  for(i=0;i<=max_digPin;i++)
    {      
      if(g_boardType==MEGA)x = c_digitalPin[i];
      if(g_boardType==UNO)x = readRegister(R_PORT,i);
      if(x > 0)
	{
	  n++;
	  tempD[0] = n;
	  tempD[n] = x;
	  pinD[n]  = i;
	} 
    }

  strcpy(z,"");
  // Mode status of digital pins
  for(i=0;i<=max_digPin;i++)
    {      
      if(g_boardType==UNO)
	{
	  in_out = readRegister(R_DDR,i);
	  if(in_out == 0)z[i]   ='I';
	  if(in_out == 1)z[i]   ='o';
	}
      else // MEGA
	{
	  if(digitalMode[i]==INPUT)
	    {
	      in_out = 0;
	      z[i]   ='I';
	    }
	  if(digitalMode[i]==OUTPUT)
	    {
	      in_out = 1;
	      z[i]   ='o';
	    }
	}


      x = digitalMode[i];
      if(in_out == 0)  // INPUT
	{
	  if(x == FREE)z[i]          ='-';
	  else if(x == CHANGE)z[i]   ='C';
	  else if(x == RISING)z[i]   ='R';
	  else if(x == FALLING)z[i]  ='F';
	  else if(x == LOW)z[i]      ='L';
	  else if(x == RX)z[i]       ='X';
	  else if(x == TX)z[i]       ='Y';
	  //else z[i] = 'Q';
	}
      else  // OUTPUT
	{
	  if(x == FREE)z[i]          ='-';
	  else if(x == CHANGE)z[i]   ='c';
	  else if(x == RISING)z[i]   ='r';
	  else if(x == FALLING)z[i]  ='f';
	  else if(x == LOW)z[i]      ='l';
	  else if(x == RX)z[i]       ='x';
	  else if(x == TX)z[i]       ='y';
	  //else z[i] = 'q';
	}
    }
  z[i]='\0';
  
  fprintf(s_log,"%d,%s,%d,%d",g_curStep,z,tempA[0],tempD[0]);
  if(tempA[0] > 0)
    {
      for(i=1;i<=tempA[0];i++)fprintf(s_log,",%d,%d",pinA[i],tempA[i]);
    }
  if(tempD[0] > 0)
    {
      for(i=1;i<=tempD[0];i++)fprintf(s_log,",%d,%d",pinD[i],tempD[i]);
    }
  fprintf(s_log,"\n");

  registerLog();
}

//====================================
void registerLog()
//====================================
{
  if(g_boardType==UNO)
    {
      setPINRegister(g_curStep);
      fprintf(r_log,"# %d,DDRB=%s,DDRC=%s,DDRD=%s,",g_curStep,int2bin(DDRB,8),int2bin(DDRC,8),int2bin(DDRD,8));
      fprintf(r_log,"PORTB=%s,PORTC=%s,PORTD=%s,",int2bin(PORTB,8),int2bin(PORTC,8),int2bin(PORTD,8));
      fprintf(r_log,"PINB=%s,PINC=%s,PIND=%s\n",int2bin(PINB,8),int2bin(PINC,8),int2bin(PIND,8));
      fprintf(r_log,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",g_curStep,DDRB,DDRC,DDRD,PORTB,PORTC,PORTD,PINB,PINC,PIND);
    }
}

//====================================
void boardInit()
//====================================
{
  int i,j;

  g_nloop = 0;

  for(i=0;i<40;i++)
    {
      for(j=0;j<MAX_TOTAL_PINS;j++)
	{
	  strcpy(g_custText[i][j],"no-text");
	}
    }

  for(i=0;i<MAX_READ;i++)
    {
      stepAtReadA[i]  = 0;
      stepAtReadD[i]  = 0;
      valueAtReadA[i] = 0;
      valueAtReadD[i] = 0;
      pinAtReadA[i]   = 0;
      pinAtReadD[i]   = 0;
    }

  for(i=0;i<=max_anaPin;i++)
    {
      anaPinPos[i]   = 0;
      c_analogPin[i] = 0;
      //strcpy(textAnalogRead[i],"void");
    }
  
  for(i=0;i<=max_digPin;i++)
    {
      digitalMode[i]  = FREE;
      digPinPos[i]    = 0;
      c_digitalPin[i] = 0;
      //strcpy(textPinModeIn[i],"void");
      //strcpy(textPinModeOut[i],"void");

      //strcpy(textDigitalWriteLow[i],"void");
      //strcpy(textDigitalWriteHigh[i],"void");

      //strcpy(textAnalogWrite[i],"void");
      //strcpy(textDigitalRead[i],"void");
    }

  for(i=0;i<max_irPin;i++)
    {
      interruptMode[i] = 0;
    }

  strcpy(interruptType[LOW],"interruptLOW");
  strcpy(interruptType[FALLING],"interruptFALLING");
  strcpy(interruptType[RISING],"interruptRISING");
  strcpy(interruptType[CHANGE],"interruptCHANGE");

/* // Values 0 to 255    */
/* // PWM: only pin 3,5,6,9,10,11  UNO */
/* // PWM: only pin 2 - 13 MEGA */
}

//====================================
void openFiles()
//====================================
{
  // Error logging
  e_log = fopen("serv.error","w");

/*   // Custom Logging */
/*   u_log = fopen("data.custom","w"); */
/*   if(u_log == NULL) */
/*     { */
/*       errorLog("Unable to open data.custom",0); */
/*     } */
/*   fprintf(u_log,"# Servuino Custom Simulation Data Version: %s\n",g_version); */

/*   // Arduino logging */
/*   a_log = fopen("data.arduino","w"); */
/*   if(a_log == NULL) */
/*     { */
/*       errorLog("Unable to open data.arduino",0); */
/*     } */
/*   fprintf(a_log,"# Servuino Arduino Simulation Data Version: %s\n",g_version); */

  // Status of pins
  f_pinmod = fopen("serv.pinmod","w");
  if(f_pinmod == NULL)
    {
      errorLog("Unable to open data.status",0);
    }
  fprintf(f_pinmod,"# Servuino Status Pin Mode Version: %s\n",g_version);

  // Status of pins
  f_digval = fopen("serv.digval","w");
  if(f_digval == NULL)
    {
      errorLog("Unable to open data.status",0);
    }
  fprintf(f_digval,"# Servuino Status Pin Digital Values Version: %s\n",g_version);

  // Status of pins
  f_anaval = fopen("serv.anaval","w");
  if(f_anaval == NULL)
    {
      errorLog("Unable to open data.status",0);
    }
  fprintf(f_anaval,"# Servuino Status Pin Analog Values Version: %s\n",g_version);

  // Status of pins
  f_pinrw = fopen("serv.pinrw","w");
  if(f_pinrw == NULL)
    {
      errorLog("Unable to open data.status",0);
    }
  fprintf(f_pinrw,"# Servuino Status Pin RW Version: %s\n",g_version);


  // Serial logging
  f_serial = fopen("serv.serial","w");
  if(f_serial == NULL)
    {
      errorLog("Unable to open serv.serial",0);
    }
  fprintf(f_serial,"# Servuino Serial Simulation Data Version: %s\n",g_version);

  // Serial logging
  f_time = fopen("serv.time","w");
  if(f_time == NULL)
    {
      errorLog("Unable to open serv.time",0);
    }
  fprintf(f_time,"# Servuino Time Delay (microseconds) Version: %s\n",g_version);

  // Register logging
  r_log = fopen("data.register","w");
  if(r_log == NULL)
    {
      errorLog("Unable to open data.register",0);
    }
  fprintf(r_log,"# Servuino Register Data Version: %s\n",g_version);
  // serv.event
  f_event = fopen("serv.event","w");
  if(f_event == NULL)
    {
      errorLog("Unable to open data.register",0);
    }
  fprintf(f_event,"# Servuino Event Log Version: %s\n",g_version);

  // serv.cust
  f_cust = fopen("serv.cust","w");
  if(f_cust == NULL)
    {
      errorLog("Unable to open data.register",0);
    }
  fprintf(f_cust,"# Servuino Cust Log Version: %s\n",g_version);

}

//====================================
void closeFiles()
//====================================
{
  fclose(f_pinmod);
  fclose(f_digval);
  fclose(f_anaval);
  fclose(f_pinrw);
  fclose(e_log);
  fclose(r_log);

  fclose(f_event);
  fclose(f_cust);
  fclose(f_time);
  fclose(f_serial);
}

//====================================
void errorLog(const char msg[], int x)
//====================================
{
  //printf("ServuinoERROR: %s %d\n",msg,x);
  fprintf(e_log,"ServuinoERROR[step %d]: %s %d\n",g_curStep,msg,x);
  return;
}

//====================================
int getAnalogPinValue(int pin,int step)
//====================================
{  
  int i,res=0,limit;

  if(g_interpolation == YES)
    {
      limit = s_analogStep[0][pin];
      for (i=1;i<=limit;i++)
	{
	  if(step >= s_analogStep[i][pin] && step < s_analogStep[i+1][pin])
	    res = s_analogPin[i][pin];
	}
      if(step >= s_analogStep[limit][pin]) res = s_analogPin[limit][pin];
    }
  else
    {
      limit = s_analogStep[0][pin];
      for (i=1;i<=limit;i++)
	{
	  if(step >= s_analogStep[i][pin] && step < s_analogStep[i+1][pin])
	    res = s_analogPin[i][pin];
	}
      if(step >= s_analogStep[limit][pin]) res = s_analogPin[limit][pin];
    }

  return(res);
}  

//====================================
int getDigitalPinValue(int pin,int step)
//====================================
{  
  int i,res=0,limit;


  limit = s_digitalStep[0][pin];

  for (i=1;i<=limit;i++)
    {
      if(step >= s_digitalStep[i][pin] && step < s_digitalStep[i+1][pin])
	res = s_digitalPin[i][pin];
    }
  if(step >= s_digitalStep[limit][pin]) res = s_digitalPin[limit][pin];
  return(res);
}  

//====================================
int insDigitalPinValue(int pin,int step, int value)
//====================================
{  
  int i,limit,hit=0,ok=0;

  ok = ok + checkRange(S_OK,"digpin",pin);
  ok = ok + checkRange(S_OK,"step",step);
  ok = ok + checkRange(S_OK,"digval",value);

  if(ok == S_OK)
    {

      limit = s_digitalStep[0][pin];

      // Insert breakpoint at the end
      if(step > s_digitalStep[limit][pin])
	{
	  hit = 1;
	  s_digitalPin[limit+1][pin]  = value;
	  s_digitalStep[limit+1][pin] = step;
	  limit++;
	}  

      // Insert breakpoint at the begining
      else if(step < s_digitalStep[1][pin])
	{
	  hit = limit;
	  for(i=limit;i>0;i--)
	    {
	      s_digitalPin[i+1][pin]  = s_digitalPin[i][pin];
	      s_digitalStep[i+1][pin] = s_digitalStep[i][pin];
	    }
	  s_digitalPin[1][pin]  = value;
	  s_digitalStep[1][pin] = step;
	  limit++;
	}    
      // Insert breakpoint in between
      else
	{
	  for (i=1;i<=limit;i++)
	    {	      
	      // If step in between
	      if(step > s_digitalStep[i][pin] && step < s_digitalStep[i+1][pin])
		{
		  hit = i+1; // New breakpoint index
		}
	      // If breakpoint already exists - replace value
	      if(step == s_digitalStep[i][pin])
		{
		  hit = -1;
		  s_digitalPin[i][pin]  = value;
		}
	    }
	  // Insert breakpoint
	  if(hit > 0)
	    {
	      for(i=limit;i>=hit;i--)
		{
		  s_digitalPin[i+1][pin]  = s_digitalPin[i][pin];
		  s_digitalStep[i+1][pin] = s_digitalStep[i][pin];
		}
	      s_digitalPin[hit][pin]  = value;
	      s_digitalStep[hit][pin] = step;
	      limit++;
	    }
	}
      s_digitalStep[0][pin] = limit;

    }
  if(hit==0)
    {
      sprintf(g_temp,"Digital: Unable to insert breakpoint pin=%d step=%d value=%d",pin,step,value);
      errorLog(g_temp,0);
    }
  return(hit);
}  

//====================================
int insAnalogPinValue(int pin,int step, int value)
//====================================
{  
  int i,limit,hit=0,ok=0;

  ok = ok + checkRange(S_OK,"anapin",pin);
  ok = ok + checkRange(S_OK,"step",step);
  ok = ok + checkRange(S_OK,"anaval",value);

  if(ok == S_OK)
    {

      limit = s_analogStep[0][pin];

      // Insert breakpoint at the end
      if(step > s_analogStep[limit][pin])
	{
	  hit = 1;
	  s_analogPin[limit+1][pin]  = value;
	  s_analogStep[limit+1][pin] = step;
	  limit++;
	}  

      // Insert breakpoint at the begining
      else if(step < s_analogStep[1][pin])
	{
	  hit = limit;
	  for(i=limit;i>0;i--)
	    {
	      s_analogPin[i+1][pin]  = s_analogPin[i][pin];
	      s_analogStep[i+1][pin] = s_analogStep[i][pin];
	    }
	  s_analogPin[1][pin]  = value;
	  s_analogStep[1][pin] = step;
	  limit++;
	}    
      // Insert breakpoint in between
      else
	{
	  for (i=1;i<=limit;i++)
	    {
	      // If step in between
	      if(step > s_analogStep[i][pin] && step < s_analogStep[i+1][pin])
		{
		  hit = i+1; // New breakpoint index
		}
	      // If breakpoint already exists - replace value
	      if(step == s_analogStep[i][pin])
		{
		  hit = -1;
		  s_analogPin[i][pin]  = value;
		}
	    }
	  // Insert breakpoint
	  if(hit > 0)
	    {
	      for(i=limit;i>=hit;i--)
		{
		  s_analogPin[i+1][pin]  = s_analogPin[i][pin];
		  s_analogStep[i+1][pin] = s_analogStep[i][pin];
		}
	      s_analogPin[hit][pin]  = value;
	      s_analogStep[hit][pin] = step;
	      limit++;
	    }
	}
      s_analogStep[0][pin] = limit;

    }
  if(hit==0)
    {
      sprintf(g_temp,"Analog: Unable to insert breakpoint pin=%d step=%d value=%d",pin,step,value);
      errorLog(g_temp,0);
    }
  return(hit);
}  


//====================================
int delDigitalPinValue(int pin,int step)
//====================================
{  
  int i,limit,hit=0,ok=0;

  ok = ok + checkRange(S_OK,"digpin",pin);
  ok = ok + checkRange(S_OK,"step",step);

  if(ok == S_OK)
    {
      limit = s_digitalStep[0][pin];
      for (i=1;i<=limit;i++)
	{
	  if(step == s_digitalStep[i][pin])
	    {
	      hit = i;
	    }
	}
      
      if(hit > 0)
	{
	  s_digitalStep[0][pin]--;
	  //Remove the only/last breakpoint
	  if(limit == hit)
	    {
	      s_digitalPin[hit][pin]  = 0;
	      s_digitalStep[hit][pin] = 0;
	    }
	  else if(hit < limit) 
	    {
	      for(i=hit;i<limit;i++)
		{
		  s_digitalPin[i][pin]  = s_digitalPin[i+1][pin];
		  s_digitalStep[i][pin] = s_digitalStep[i+1][pin];
		}
	    }
	}
    }
  if(hit==0)
    {
      sprintf(g_temp,"Digital: Unable to delete breakpoint pin=%d step=%d",pin,step);
      errorLog(g_temp,0);
    }
  return(hit);
}  


//====================================
int delAnalogPinValue(int pin,int step)
//====================================
{  
  int i,limit,hit=0,ok=0;

  ok = ok + checkRange(S_OK,"anapin",pin);
  ok = ok + checkRange(S_OK,"step",step);

  if(ok == S_OK)
    {
      limit = s_analogStep[0][pin];
      for (i=1;i<=limit;i++)
	{
	  if(step == s_analogStep[i][pin])
	    {
	      hit = i;
	    }
	}
      
      if(hit > 0)
	{
	  s_analogStep[0][pin]--;
	  //Remove the only/last breakpoint
	  if(limit == hit)
	    {
	      s_analogPin[hit][pin]  = 0;
	      s_analogStep[hit][pin] = 0;
	    }
	  else if(hit < limit) 
	    {
	      for(i=hit;i<limit;i++)
		{
		  s_analogPin[i][pin]  = s_analogPin[i+1][pin];
		  s_analogStep[i][pin] = s_analogStep[i+1][pin];
		}
	    }
	}
    }
  if(hit==0)
    {
      sprintf(g_temp,"Analog: Unable to delete breakpoint pin=%d step=%d",pin,step);
      errorLog(g_temp,0);
    }
  return(hit);
}  


//====================================
int checkScenario(int now,int anadig,int pin, int step)
//====================================
{
  int k;
  int hit = 0,send;

  if(anadig == DIG)
    {
      send = s_digitalStep[0][pin];
      for(k=now+1;k<=send;k++)
	{
	  if(step == s_digitalStep[k][pin])hit++;
	}
    }

  if(anadig == ANA)
    {
      send = s_analogStep[0][pin];
      for(k=now+1;k<=send;k++)
	{
	  if(step == s_digitalStep[k][pin])hit++;
	}
    } 
  return(hit);
}

//====================================
void saveScenario() // Servuino input/output
//====================================
{
  int i,j,k,step,ok=0;
  FILE *out;

  out = fopen("data.scen","w");
  if(out == NULL)
    {
      errorLog("Unable to open data.scen",0);
    }
  
  for(i=0;i<=max_digPin;i++)
    {
      for(k=1;k<=s_digitalStep[0][i];k++)
	{
	  step = s_digitalStep[k][i];
	  ok = checkScenario(k,DIG,i,step);
	  if(ok==0)
	    fprintf(out,"// SCENDIGPIN %d %d %d\n",i,s_digitalStep[k][i],s_digitalPin[k][i]);
	}
    }
  
  for(i=0;i<=max_anaPin;i++)
    {
      for(k=1;k<=s_analogStep[0][i];k++)
	{
	  step = s_analogStep[k][i];
	  ok = checkScenario(k,ANA,i,step);
	  if(ok==0)
	    fprintf(out,"// SCENANAPIN %d %d %d\n",i,s_analogStep[k][i],s_analogPin[k][i]);
	}
    }
  
  fclose(out);
  return;
}



//====================================
void saveScenarioExpanded() // Servuino input/output
//====================================
{
  int i,j,k,temp;
  FILE *out;

  out = fopen("data.scenario","w");
  if(out == NULL)
    {
      errorLog("Unable to open data.scenario",0);
    }
  fprintf(out,"# Digital: "); 
  for(i=0;i<=max_digPin;i++)
    {
      if(s_digitalStep[0][i] > 0)
	fprintf(out,"%4d ",i);
    }
  fprintf(out,"\n");
  fprintf(out,"# Analog: "); 
  for(i=0;i<=max_anaPin;i++)
    {
      if(s_analogStep[0][i] > 0)
	fprintf(out,"%4d ",i);
    }
  fprintf(out,"\n");

  for(k=1;k<=g_simulationLength;k++)
    {
      fprintf(out,"%4d          ",k);     
      for(i=0;i<=max_digPin;i++)
	{
	  if(s_digitalStep[0][i] > 0)
	    {
	      temp = getDigitalPinValue(i,k);
	      fprintf(out,"%4d ",temp);
	      x_pinScenario[i][k] = temp;
	    }
	}
      fprintf(out,"       ");
      for(i=0;i<=max_anaPin;i++)
	{
	  if(s_analogStep[0][i] > 0)
	    {
	      temp = getAnalogPinValue(i,k);
	      fprintf(out,"%4d ",temp);
	      x_pinScenario[i+g_nDigPins][k] = temp;
	    }
	}
      fprintf(out,"\n");      
    }
 
  fclose(out);
  return;
}

//====================================
void setPINRegister(int step) 
//====================================
{
  int i,temp;

  for(i=0;i<=max_digPin;i++)
    {
      temp = getDigitalPinValue(i,step);
      writeRegister(1,R_PIN,i,temp);
    }
/*   for(i=0;i<=max_anaPin;i++) */
/*     { */
/*       temp = getAnalogPinValue(i,step); */
/*       writeRegister(0,R_PIN,i,temp); */
/*     } */
  return;
}


/* //==================================== */
/* void iLog1(const char *p, int value1) */
/* //==================================== */
/* { */
/*   fprintf(a_log,"* %d %s ? %d\n",currentStep,p,value1); */
/*   fprintf(u_log,"* %d %s ? %d\n",currentStep,p,value1); */
/*   return; */
/* } */

/* //==================================== */
/* void mLine() */
/* //==================================== */
/* { */
/*   char line[120]; */
  
/*   strcpy(line,"--------------------"); */
/*   fprintf(a_log,"= %d ? %s\n",currentStep,line); */
/*   fprintf(u_log,"= %d ? %s\n",currentStep,line); */
/*   return; */
/* } */
/* //==================================== */
/* void mLineText(const char *t) */
/* //==================================== */
/* { */
/*   char line[120]; */
  
/*   sprintf(line,"------ %s ------",t); */
/*   fprintf(a_log,"= %d ? %s\n",currentStep,line); */
/*   fprintf(u_log,"= %d ? %s\n",currentStep,line); */
/*   return; */
/* } */
/* //==================================== */
/* void mLog0(const char *p) */
/* //==================================== */
/* { */
/*   fprintf(a_log,"= %d ? %s\n",currentStep,p); */
/*   fprintf(u_log,"= %d ? %s\n",currentStep,p); */
/*   return; */
/* } */
/* //==================================== */
/* void mLog1(const char *p, int value1) */
/* //==================================== */
/* { */
/*   fprintf(a_log,"= %d ? %s %d\n",currentStep,p,value1); */
/*   fprintf(u_log,"= %d ? %s %d\n",currentStep,p,value1); */
/*   return; */
/* } */



//====================================
int wCustomLog(char *in, char *out)
//====================================
{
  char *q,*p;
  int pin;

  p = strstr(in,":");
  p++;
  sscanf(p,"%d",&pin);
  p = strstr(p,"\"");
  p++;
  q = strstr(p,"\"");
  strcpy(q,"\0");
  strcpy(out,p);

  return(pin);
}


//====================================
void readSketchInfo()
//====================================
{
  FILE *in;
  char row[120],res[40],*p,*q,value[5];
  int pin;

  in = fopen("sketch.pde","r");
  if(in == NULL)
    {
      errorLog("Error: Unable to open sketch",g_curStep);
    }
  else
    {
      while (fgets(row,120,in)!=NULL)
	{
	  if(row[0] == '/')
	    {
	      if(p=strstr(row,"SKETCH_NAME:"))
		{
		  fprintf(f_event,"#%s",row);
		  q = strstr(p,":");q++;
		  sscanf(q,"%s",appName);
		}
	      if(p=strstr(row,"BOARD_TYPE"))
		{
		  fprintf(f_event,"#%s",row);
		  if(strstr(row,"UNO") != NULL) g_boardType = UNO;
		  if(strstr(row,"MEGA")!= NULL) g_boardType = MEGA;
		}
	    }
	}
    }
  fclose(in);  
}


//====================================
void stopEncoding()
//====================================
{

  fprintf(f_event,"# ENDOFSIM\n");
  saveScenario();
  //saveScenarioExpanded();
  closeFiles();
  exit(0);
}
//====================================
void savePinStatus()
//====================================
{
  int i;
  for(i=0;i<=max_digPin;i++)
    s_digitalPin[g_curStep][i] = c_digitalPin[i];
  for(i=0;i<=max_anaPin;i++)
    s_analogPin[g_curStep][i]  = c_analogPin[i];
}

//====================================
void doInterrupt(int pin,int ir, int irType,int value)
//====================================
{
  if(g_allowInterrupt == NO)
    {
      errorLog("Try to interrupt within an interrupt",g_curStep);
      return;
    }

  //mLog1(interruptType[irType],ir);
  //mLineText("interrupt in");
  g_allowInterrupt = NO;
  interrupt[ir]();  
  g_allowInterrupt = YES;
  //mLineText("interrupt out");
}

//====================================
void interruptNow()
//====================================
{
  int ir,ir_1,ir_2,pin,extTrigged = NO;

  for(ir=0;ir<=max_irPin;ir++)
    {
      if(attached[ir] == YES)
	{
	  pin = inrpt[ir];

	  ir_1 = getDigitalPinValue(pin,g_curStep);
	  ir_2 = getDigitalPinValue(pin,g_curStep-1);
	  
	  if(interruptMode[ir] == RISING && ir_1 == 1 && ir_2 == 0)
	    {
	      doInterrupt(pin,ir,RISING,1);
	    }
	  if(interruptMode[ir] == FALLING && ir_1 == 0 && ir_2 == 1)
	    {
	      doInterrupt(pin,ir,FALLING,0);
	    }
	  if(interruptMode[ir] == CHANGE && ir_1 != ir_2)
	    {
	      doInterrupt(pin,ir,CHANGE,ir_1);
	    }
	  if(interruptMode[ir] == LOW && ir_1 != ir_2)
	    {
	      doInterrupt(pin,ir,LOW,ir_1);
	    }
	}
    } 
}
//====================================
void readScenario()
//====================================
{
  FILE *in;
  char row[120],*p, junk[20];
  int pin,step,value,i,j;
  int tmp=0,dCount[MAX_PIN_DIGITAL_MEGA],aCount[MAX_PIN_ANALOG_MEGA];

  if(g_scenSource == 0)in = fopen("sketch.pde","r");
  if(g_scenSource == 1)in = fopen("data.scen","r");

  for(i=0;i<=max_anaPin;i++)
    {
      for(j=0;j<SCEN_MAX;j++)
	{
	  s_analogPin[j][i]  = 0;
	  s_analogStep[j][i] = 0;
	} 
    }
  
  for(i=0;i<=max_digPin;i++)
    {
      for(j=0;j<SCEN_MAX;j++)
	{
	  s_digitalPin[j][i]  = 0;
	  s_digitalStep[j][i] = 0;
	}
    }

  for(i=0;i<=max_digPin;i++)dCount[i] = 0;
  for(i=0;i<=max_anaPin;i++)aCount[i] = 0;

  if(in == NULL)
    {
      errorLog("Unable to open sketch for scenario reading",0);
    }
  else
    {
      while (fgets(row,120,in)!=NULL)
	{
/* 	  if(p=strstr(row,"SCENLENGTH")) */
/* 	    { */
/* 	      sscanf(p,"%s%d",junk,&g_simulationLength); */
/* 	    } */
	  if(p=strstr(row,"SCENDIGPIN"))
	    {
	      sscanf(p,"%s%d%d%d",junk,&pin,&step,&value);
              pin   = checkRange(HEAL,"digpin",pin);
              value = checkRange(HEAL,"digval",value);
              step  = checkRange(HEAL,"step",step);
	      dCount[pin]++;
	      tmp = dCount[pin];
	      if(step < s_digitalStep[tmp-1][pin])
		fprintf(e_log,"Error:Scenario data not given in increasing order: Digital Step=%d Pin=%d\n",step,pin);
	      s_digitalStep[tmp][pin]  = step;
	      s_digitalPin[tmp][pin]   = value;
	      s_digitalStep[0][pin]    = tmp;
	    }
	  if(p=strstr(row,"SCENANAPIN"))
	    {
	      sscanf(p,"%s%d%d%d",junk,&pin,&step,&value);
              pin   = checkRange(HEAL,"anapin",pin);
              value = checkRange(HEAL,"anaval",value);
              step  = checkRange(HEAL,"step",step);
	      aCount[pin]++;
	      tmp = aCount[pin];
	      if(step < s_analogStep[tmp-1][pin])
		fprintf(e_log,"Error:Scenario data not given in increasing order: Analog Step=%d Pin=%d\n",step,pin);
	      s_analogStep[tmp][pin]   = step;
	      s_analogPin[tmp][pin]    = value;
	      s_analogStep[0][pin]     = tmp;
	    }
	}
    }
}

//====================================
void readCustom()
//====================================
{
  FILE *in;
  char row[80],res[40],*p,*q,value[5];
  int pin;

  in = fopen("sketch.pde","r");
  if(in == NULL)
    {
      errorLog("No sketch.pde",0);
    }
  else
    {
      while (fgets(row,80,in)!=NULL)
	{
	  if(p=strstr(row,"PINMODE_IN:"))
	    {
	      pin = wCustomLog(p,res);
	      //strcpy(textPinModeIn[pin],res);
	      strcpy(g_custText[S_PIN_MODE_INPUT][pin],res);
	    }
	  if(p=strstr(row,"PINMODE_OUT:"))
	    {
	      pin = wCustomLog(p,res);
	      //strcpy(textPinModeOut[pin],res);
	      strcpy(g_custText[S_PIN_MODE_OUTPUT][pin],res);
	    }
	  if(p=strstr(row,"DIGITALWRITE_LOW:"))
	    {
	      pin = wCustomLog(p,res);
	      //strcpy(textDigitalWriteLow[pin],res);
	      strcpy(g_custText[S_DIGITAL_WRITE_LOW][pin],res);
	    }
	  if(p=strstr(row,"DIGITALWRITE_HIGH:"))
	    {
	      pin = wCustomLog(p,res);
	      //strcpy(textDigitalWriteHigh[pin],res);
	      strcpy(g_custText[S_DIGITAL_WRITE_HIGH][pin],res);
	    }
	  if(p=strstr(row,"ANALOGREAD:"))
	    {
	      pin = wCustomLog(p,res);
	      //strcpy(textAnalogRead[pin],res);
	      strcpy(g_custText[S_ANALOG_READ][pin],res);
	    }
	  if(p=strstr(row,"DIGITALREAD:"))
	    {
	      pin = wCustomLog(p,res);
	      //strcpy(textDigitalRead[pin],res);
	      strcpy(g_custText[S_DIGITAL_READ][pin],res);
	    }
	  if(p=strstr(row,"ANALOGWRITE:"))
	    {
	      pin = wCustomLog(p,res);
	      //strcpy(textAnalogWrite[pin],res);
	      strcpy(g_custText[S_ANALOG_WRITE][pin],res);
	    }
	}
      fclose(in); 
    }
}

//====================================
// End of file
//====================================
