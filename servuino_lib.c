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
void codeLog(int fn,int a,int b, int c, int d,const char *p)
//====================================
{
  fprintf(c_log,"+ %d %d %d %d %d %d '%s'\n",currentStep,fn,a,b,c,d,p);
}

//====================================
void statusLog()
//====================================
{
  int x,n,i;
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
      x = c_digitalPin[i];
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
      x = digitalMode[i];
      if(x == FREE)z[i]   ='-';
      else if(x == INPUT)z[i]  ='I';
      else if(x == OUTPUT)z[i] ='O';
      else if(x == CHANGE)z[i] ='C';
      else if(x == RISING)z[i] ='R';
      else if(x == FALLING)z[i]='F';
      else if(x == RX)z[i]     ='X';
      else if(x == TX)z[i]     ='Y';
      else z[i] = 'Q';
    }
  z[i]='\0';
  
  fprintf(s_log,"%d,%s,%d,%d",currentStep,z,tempA[0],tempD[0]);
  if(tempA[0] > 0)
    {
      for(i=1;i<=tempA[0];i++)fprintf(s_log,",%d,%d",pinA[i],tempA[i]);
    }
  if(tempD[0] > 0)
    {
      for(i=1;i<=tempD[0];i++)fprintf(s_log,",%d,%d",pinD[i],tempD[i]);
    }
  fprintf(s_log,"\n");
}

//====================================
void boardInit()
//====================================
{
  int i,j;

  g_nloop = 0;

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
      strcpy(textAnalogRead[i],"void");
    }
  
  for(i=0;i<=max_digPin;i++)
    {
      digitalMode[i]  = FREE;
      digPinPos[i]    = 0;
      c_digitalPin[i] = 0;
      strcpy(textPinModeIn[i],"void");
      strcpy(textPinModeOut[i],"void");

      strcpy(textDigitalWriteLow[i],"void");
      strcpy(textDigitalWriteHigh[i],"void");

      strcpy(textAnalogWrite[i],"void");
      strcpy(textDigitalRead[i],"void");
    }

  for(i=0;i<max_irPin;i++)
    {
      interruptMode[i] = 0;
    }
}

//====================================
void openFiles()
//====================================
{
  // Error logging
  e_log = fopen("data.error","w");

  // Custom Logging
  u_log = fopen("data.custom","w");
  if(u_log == NULL)
    {
      errorLog("Unable to open data.custom",0);
    }
  fprintf(u_log,"# Servuino Custom Simulation Data Version: %s\n",g_version);

  // Arduino logging
  a_log = fopen("data.arduino","w");
  if(a_log == NULL)
    {
      errorLog("Unable to open data.arduino",0);
    }
  fprintf(a_log,"# Servuino Arduino Simulation Data Version: %s\n",g_version);

  // Status of pins
  s_log = fopen("data.status","w");
  if(s_log == NULL)
    {
      errorLog("Unable to open data.status",0);
    }
  fprintf(s_log,"# Servuino Status Simulation Data Version: %s\n",g_version);

  // Coded logging
  c_log = fopen("data.code","w");
  if(c_log == NULL)
    {
      errorLog("Unable to open data.code",0);
    }
  fprintf(c_log,"# Servuino Code Simulation Data Version: %s\n",g_version);

  // Serial logging
  x_log = fopen("data.serial","w");
  if(x_log == NULL)
    {
      errorLog("Unable to open data.serial",0);
    }
  fprintf(x_log,"# Servuino Serial Simulation Data Version: %s\n",g_version);


}

//====================================
void closeFiles()
//====================================
{
  fclose(s_log);
  fclose(e_log);
  fclose(c_log);
  fclose(u_log);
  fclose(a_log);
  fclose(x_log);
}

//====================================
void errorLog(const char msg[], int x)
//====================================
{
  fprintf(e_log,"ServuinoERROR: %s %d\n",msg,x);
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
      for (i=0;i<limit;i++)
	{
	  if(step >= s_analogStep[i][pin] && step < s_analogStep[i+1][pin])
	    res = s_analogPin[i][pin];
	}
      if(step >= s_analogStep[limit][pin]) res = s_analogPin[limit][pin];
    }
  else
    {
      limit = s_analogStep[0][pin];
      for (i=0;i<limit;i++)
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

  for (i=0;i<limit;i++)
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
      
      for (i=1;i<limit;i++)
	{
	  if(step > s_digitalStep[i][pin] && step < s_digitalStep[i+1][pin])
	    {
	      hit = i+1;
	    }
	  if(step == s_digitalStep[i][pin])
	    {
	      hit = 0;
	      s_digitalPin[i][pin]  = value;
	      s_digitalStep[i][pin] = step;
	    }
	}
      if(step > s_digitalStep[limit][pin])hit = limit+1;
      
      
      if(hit > 0)
	{
	  s_digitalStep[0][pin]++;
	  limit = s_digitalStep[0][pin];
	  for(i=limit;i>=hit;i--)
	    {
	      s_digitalPin[i][pin]  = s_digitalPin[i-1][pin];
	      s_digitalStep[i][pin] = s_digitalStep[i-1][pin];
	    }
	  s_digitalPin[hit][pin]  = value;
	  s_digitalStep[hit][pin] = step;
	}
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
      
      for (i=1;i<limit;i++)
	{
	  if(step > s_analogStep[i][pin] && step < s_analogStep[i+1][pin])
	    {
	      hit = i+1;
	    }
	  if(step == s_analogStep[i][pin])
	    {
	      hit = 0;
	      s_analogPin[i][pin]  = value;
	      s_analogStep[i][pin] = step;
	    }
	}
      if(step > s_analogStep[limit][pin])hit = limit+1;
      
      
      if(hit > 0)
	{
	  s_analogStep[0][pin]++;
	  limit = s_analogStep[0][pin];
	  for(i=limit;i>=hit;i--)
	    {
	      s_analogPin[i][pin]  = s_analogPin[i-1][pin];
	      s_analogStep[i][pin] = s_analogStep[i-1][pin];
	    }
	  s_analogPin[hit][pin]  = value;
	  s_analogStep[hit][pin] = step;
	}
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
	  for(i=hit;i<limit;i++)
	    {
	      s_digitalPin[i][pin]  = s_digitalPin[i+1][pin];
	      s_digitalStep[i][pin] = s_digitalStep[i+1][pin];
	    }
	}
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
	  for(i=hit;i<limit;i++)
	    {
	      s_analogPin[i][pin]  = s_analogPin[i+1][pin];
	      s_analogStep[i][pin] = s_analogStep[i+1][pin];
	    }
	}
    }
  return(hit);
}  


//====================================
void saveScenario() // Servuino input/output
//====================================
{
  int i,j,k;
  FILE *out;

  out = fopen("data.scen","w");
  if(out == NULL)
    {
      fprintf(e_log,"Unable to open data.scen\n");
    }
  
  for(i=0;i<=max_digPin;i++)
    {
      for(k=1;k<=s_digitalStep[0][i];k++)
	{
	  fprintf(out,"// SCENDIGPIN %d %d %d\n",i,s_digitalStep[k][i],s_digitalPin[k][i]);
	}
    }
  
  for(i=0;i<=max_anaPin;i++)
    {
      for(k=1;k<=s_analogStep[0][i];k++)
	{
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
      fprintf(e_log,"Unable to open data.scenario\n");
    }
  fprintf(out,"Step  Digital "); 
  for(i=0;i<=max_digPin;i++)
    {
      if(s_digitalStep[0][i] > 0)
	fprintf(out,"%4d ",i);
    }
  fprintf(out,"Analog "); 
  for(i=0;i<=max_anaPin;i++)
    {
      if(s_analogStep[0][i] > 0)
	fprintf(out,"%4d ",i);
    }
  fprintf(out,"\n");
  fprintf(out,"----\n");

  for(k=1;k<=g_simulationLength;k++)
    {
      fprintf(out,"%4d          ",k);     
      for(i=0;i<=max_digPin;i++)
	{
	  if(s_digitalStep[0][i] > 0)
	    {
	      temp = getDigitalPinValue(i,k);
	      fprintf(out,"%4d ",temp);
	    }
	}
      fprintf(out,"       ");
      for(i=0;i<=max_anaPin;i++)
	{
	  if(s_analogStep[0][i] > 0)
	    {
	      temp = getAnalogPinValue(i,k);
	      fprintf(out,"%4d ",temp);
	    }
	}
      fprintf(out,"\n");      
    }
 
  fclose(out);
  return;
}


//====================================
void iLog1(const char *p, int value1)
//====================================
{
  fprintf(a_log,"* %d %s %d\n",currentStep,p,value1);
  fprintf(u_log,"* %d %s %d\n",currentStep,p,value1);
  return;
}

//====================================
void mLine()
//====================================
{
  char line[120];
  
  strcpy(line,"--------------------");
  fprintf(a_log,"= %d %s\n",currentStep,line);
  fprintf(u_log,"= %d %s\n",currentStep,line);
  return;
}
//====================================
void mLineText(const char *t)
//====================================
{
  char line[120];
  
  sprintf(line,"------ %s ------",t);
  fprintf(a_log,"= %d %s\n",currentStep,line);
  fprintf(u_log,"= %d %s\n",currentStep,line);
  return;
}
//====================================
void mLog0(const char *p)
//====================================
{
  fprintf(a_log,"= %d %s\n",currentStep,p);
  fprintf(u_log,"= %d %s\n",currentStep,p);
  return;
}
//====================================
void mLog1(const char *p, int value1)
//====================================
{
  fprintf(a_log,"= %d %s %d\n",currentStep,p,value1);
  fprintf(u_log,"= %d %s %d\n",currentStep,p,value1);
  return;
}

//====================================
void wLog0(int au,const char *p)
//====================================
{
  if(au==0)fprintf(a_log,"+ %d %s\n",currentStep,p);
  if(au==1)fprintf(u_log,"+ %d %s\n",currentStep,p);
  return;
}

//====================================
void wLog1(int au,const char *p, int value1)
//====================================
{
  if(au==0)fprintf(a_log,"+ %d %s %d\n",currentStep,p,value1);
  if(au==1)fprintf(u_log,"+ %d %s %d\n",currentStep,p,value1);
  return;
}

//====================================
void wLog2(int au,const char *p, int value1, int value2)
//====================================
{
  if(au==0)fprintf(a_log,"+ %d %s %d %d\n",currentStep,p,value1,value2);
  if(au==1)fprintf(u_log,"+ %d %s %d %d\n",currentStep,p,value1,value2);
  return;
}


//====================================
void wLogChar1(int au,const char *p, const char *value1)
//====================================
{
  if(au==0)fprintf(a_log,"+ %d %s '%s'\n",currentStep,p,value1);
  if(au==1)fprintf(u_log,"+ %d %s '%s'\n",currentStep,p,value1);
  return;
}

//====================================
void wLogChar2(int au,const char *p, const char *value1, int value2)
//====================================
{
  if(au==0)fprintf(a_log,"+ %d %s '%s' %d\n",currentStep,p,value1,value2);
  if(au==1)fprintf(u_log,"+ %d %s '%s' %d\n",currentStep,p,value1,value2);
  return;
}

//====================================
void showSerial(const char *m, int newLine)
//====================================
{
  int i;
  char stemp[120];

  if(serialMode == ON)
    {
      strcpy(stemp,"Serial");
      sprintf(stemp,"%s",m);
      //printf("%s\n",m);
      wLog0(0,m);
    }
  else
    {
      mLog1("Serial output without Serial.begin",currentStep);
    }
}

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
      errorLog("Error: Unable to open sketch",currentStep);
    }
  else
    {
      while (fgets(row,120,in)!=NULL)
	{
	  if(row[0] == '/')
	    {
	      if(p=strstr(row,"SKETCH_NAME:"))
		{
		  fprintf(a_log,"#%s",row);
		  q = strstr(p,":");q++;
		  sscanf(q,"%s",appName);
		}
	      if(p=strstr(row,"BOARD_TYPE"))
		{
		  fprintf(a_log,"#%s",row);
		  if(strstr(row,"UNO") != NULL) boardType = UNO;
		  if(strstr(row,"MEGA")!= NULL) boardType = MEGA;
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

  fprintf(a_log,"# ENDOFSIM\n");
  saveScenario();
  saveScenarioExpanded();
  closeFiles();
  exit(0);
}
//====================================
void savePinStatus()
//====================================
{
  int i;
  for(i=0;i<=max_digPin;i++)
    s_digitalPin[currentStep][i] = c_digitalPin[i];
  for(i=0;i<=max_anaPin;i++)
    s_analogPin[currentStep][i]  = c_analogPin[i];
}
//====================================
void passTime()
//====================================
{
  statusLog();
  currentStep++;
  if(g_simulationLength < currentStep)stopEncoding();
  return;
}


//====================================
void doInterrupt(int pin,int ir, int irType,int value)
//====================================
{
  if(g_allowInterrupt == NO)
    {
      errorLog("Try to interrupt within an interrupt",currentStep);
      return;
    }

  mLog1(interruptType[irType],ir);
  mLineText("interrupt in");
  g_allowInterrupt = NO;
  interrupt[ir]();  
  g_allowInterrupt = YES;
  mLineText("interrupt out");
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

	  ir_1 = getDigitalPinValue(pin,currentStep);
	  ir_2 = getDigitalPinValue(pin,currentStep-1);
	  
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
	  if(p=strstr(row,"SCENLENGTH"))
	    {
	      sscanf(p,"%s%d",junk,&g_simulationLength);
	    }
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
void readCustomLog()
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
	      strcpy(textPinModeIn[pin],res);
	    }
	  if(p=strstr(row,"PINMODE_OUT:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textPinModeOut[pin],res);
	    }
	  if(p=strstr(row,"DIGITALWRITE_LOW:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textDigitalWriteLow[pin],res);
	    }
	  if(p=strstr(row,"DIGITALWRITE_HIGH:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textDigitalWriteHigh[pin],res);
	    }
	  if(p=strstr(row,"ANALOGREAD:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textAnalogRead[pin],res);
	    }
	  if(p=strstr(row,"DIGITALREAD:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textDigitalRead[pin],res);
	    }
	  if(p=strstr(row,"ANALOGWRITE:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textAnalogWrite[pin],res);
	    }
	}
      fclose(in); 
    }
}

//====================================
// End of file
//====================================
