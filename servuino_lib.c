/*  Servuino is a Arduino Simulator Engine
    Copyright (C) 2011  Benny Saxen

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

  for(i=0;i<ANAPINS;i++)
    {
      anaPinPos[i]   = 0;
      c_analogPin[i] = 0;
      for(j=0;j<SCEN_MAX;j++)
	{
	  s_analogPin[j][i] = 0;
	} 
    }
  
  for(i=0;i<DIGPINS;i++)
    {
      digitalMode[i]  = FREE;
      digPinPos[i]    = 0;
      c_digitalPin[i] = 0;
      for(j=0;j<SCEN_MAX;j++)
	{
	  s_digitalPin[j][i] = 0;
	}
    }

  for(i=0;i<INTPINS;i++)
    {
      interruptMode[i] = 0;
      intPinPos[i]     = 0;
      c_intPin[i]      = 0;
      for(j=0;j<SCEN_MAX;j++)
	{
	  s_interrupt[j][i] = 0;
	}
    }

  for(i=0;i<SCEN_MAX;i++)
    {
      s_interruptStep[i] = 0;
      s_digitalStep[i]   = 0;
      s_analogStep[i]    = 0;
    }

}

//====================================
void openSimFile()
//====================================
{
  s_log = fopen("data.su","w");
  if(s_log == NULL)
    {
      printf("Unable to open data.su\n");
    }
  fprintf(s_log,"# Servuino simulation data\n");

  e_log = fopen("data.error","w");
}

//====================================
void closeSimFile()
//====================================
{
  fclose(s_log);
  fclose(e_log);
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
  int i,res=0;

  // User inserted
  if (step == g_pinStep && pin == g_pinNo && g_pinType == ANA)
    {
      return(g_pinValue);
    }

  // Based on sketch
  for (i=0;i<scenAnalog;i++)
    {
      if(step >= s_analogStep[i] && step < s_analogStep[i+1])
	res = s_analogPin[i][pin];
    }
  if(step >= s_analogStep[scenAnalog]) res = s_analogPin[scenAnalog][pin];

  return(res);
}  

//====================================
int getDigitalPinValue(int pin,int step)
//====================================
{  
  int i,res=0;

  // User inserted
  if (step == g_pinStep-1 && pin == g_pinNo && g_pinType == DIG)
    {
      return(g_pinValue);
    }

  // Based on Sketch
  for (i=0;i<scenDigital;i++)
    {
      if(step >= s_digitalStep[i] && step < s_digitalStep[i+1])
	res = s_digitalPin[i][pin];
    }
  if(step >= s_digitalStep[scenDigital]) res = s_digitalPin[scenDigital][pin];
  return(res);
}  

/* //==================================== */
/* int getInterruptValue(int pin,int step) */
/* //==================================== */
/* {   */
/*   int i,res=0; */
/*   for (i=0;i<scenInterrupt;i++) */
/*     { */
/*       if(step >= s_interruptStep[i] && step < s_interruptStep[i+1]) */
/* 	res = s_interrupt[i][pin]; */
/*     } */
/*   if(step >= s_interruptStep[scenInterrupt]) res = s_interrupt[scenInterrupt][pin]; */

/*   return(res); */
/* }   */

//====================================
void scenario()
//====================================
{
  int i,j;

  for(i=1;i<=scenDigital;i++)
    {
      fprintf(s_log,"#// SCENDIGPIN %4d ",s_digitalStep[i]);
      for(j=0;j<DIGPINS;j++)
	fprintf(s_log,"%d ",s_digitalPin[i][j]);
      fprintf(s_log,"\n");
    }

  for(i=1;i<=scenAnalog;i++)
    {
      fprintf(s_log,"#// SCENANAPIN %4d ",s_analogStep[i]);
      for(j=0;j<ANAPINS;j++)
	fprintf(s_log,"%4d ",s_analogPin[i][j]);
      fprintf(s_log,"\n");
    }

  return;
}


//====================================
void dumpReadStatistics()
//====================================
{
  int i,j;
  FILE *out;

  out = fopen("data.scen","w");
  if(out == NULL)
    {
      fprintf(e_log,"Unable to open data.scen\n");
    }


  // Based on reads  
  for(j=1;j<=stepAtReadA[0];j++)
    {
      fprintf(out,"// SCENANAPIN %d ",stepAtReadA[j]);
      for(i=0;i<ANAPINS;i++)
	{
	  if(pinAtReadA[j]== i)
	    fprintf(out,"%4d ",valueAtReadA[j]);
	  else
	    fprintf(out,"%4d ",0);
	}
      fprintf(out,"\n");
    }


  // Based on sketch

/*   for(i=1;i<=scenDigital;i++) */
/*     { */
/*       fprintf(out,"// SCENDIGPIN %4d ",s_digitalStep[i]); */
/*       for(j=0;j<DIGPINS;j++) */
/* 	fprintf(out,"%d ",s_digitalPin[i][j]); */
/*       fprintf(out,"\n"); */
/*     } */

  for(j=1;j<=stepAtReadD[0];j++)
    {
      fprintf(out,"// SCENDIGPIN %d ",stepAtReadD[j]);
      for(i=0;i<DIGPINS;i++)
	{
	  if(pinAtReadD[j]== i)
	    fprintf(out,"%4d ",valueAtReadD[j]);
	  else
	    fprintf(out,"%4d ",0);
	}
      fprintf(out,"\n");
    }
  fclose(out);
  return;
}

//====================================
void status()
//====================================
{
  int i;

  fprintf(s_log,"# STATUS %d\n",g_nloop);
  fprintf(s_log,"# DIG_PIN_MOD: ");
  for(i=0;i<13;i++)fprintf(s_log,"%3d ",digitalMode[i]);
  fprintf(s_log,"\n");
  fprintf(s_log,"# DIG_PIN_VAL: ");
  for(i=0;i<13;i++)fprintf(s_log,"%3d ",c_digitalPin[i]);
  fprintf(s_log,"\n");


  fprintf(s_log,"# ANA_PIN_VAL: ");
  for(i=0;i<6;i++) fprintf(s_log,"%3d ",c_analogPin[i]);
  fprintf(s_log,"\n");

  return;
}
//====================================
void mLine()
//====================================
{
  char line[120];
  
  strcpy(line,"--------------------");
  fprintf(s_log,"= %d %s\n",timeFromStart,line);
  return;
}
//====================================
void mLineText(const char *t)
//====================================
{
  char line[120];
  
  sprintf(line,"------ %s ------",t);
  fprintf(s_log,"= %d %s\n",timeFromStart,line);
  return;
}
//====================================
void mLog0(const char *p)
//====================================
{
  fprintf(s_log,"= %d %s\n",timeFromStart,p);
  return;
}
//====================================
void mLog1(const char *p, int value1)
//====================================
{
  fprintf(s_log,"= %d %s %d\n",timeFromStart,p,value1);
  return;
}

//====================================
void wLog0(const char *p)
//====================================
{
  fprintf(s_log,"+ %d %s\n",timeFromStart,p);
  return;
}

//====================================
void wLog1(const char *p, int value1)
//====================================
{
  fprintf(s_log,"+ %d %s %d\n",timeFromStart,p,value1);
  return;
}

//====================================
void wLog2(const char *p, int value1, int value2)
//====================================
{
  fprintf(s_log,"+ %d %s %d %d\n",timeFromStart,p,value1,value2);
  return;
}


//====================================
void wLogChar1(const char *p, const char *value1)
//====================================
{
  fprintf(s_log,"+ %d %s '%s'\n",timeFromStart,p,value1);
  return;
}

//====================================
void wLogChar2(const char *p, const char *value1, int value2)
//====================================
{
  fprintf(s_log,"+ %d %s '%s' %d\n",timeFromStart,p,value1,value2);
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
      wLog0(m);
    }
  else
    {
      mLog1("Serial output without Serial.begin",timeFromStart);
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
      errorLog("Error: Unable to open sketch",timeFromStart);
    }
  else
    {
      while (fgets(row,120,in)!=NULL)
	{
	  if(row[0] == '/')
	    {
	      if(p=strstr(row,"SKETCH_NAME:"))
		{
		  fprintf(s_log,"#%s",row);
		  q = strstr(p,":");q++;
		  sscanf(q,"%s",appName);
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
  fprintf(s_log,"# ENDOFSIM\n");
  status();
  dumpReadStatistics();
  closeSimFile();
  exit(0);
}

//====================================
void passTime()
//====================================
{
  timeFromStart++;
  if(g_simulationLength < timeFromStart)stopEncoding();
  return;
}

//====================================
void regAnaRead(int pin, int value)
//====================================
{
  int x;
  x = stepAtReadA[0];
  x++;
  stepAtReadA[0] = x;
  if(x < MAX_READ)
    {
      stepAtReadA[x]  = timeFromStart;
      valueAtReadA[x] = value;
      pinAtReadA[x]   = pin;
    }
  else
    errorLog("Number of analog events in scenario out of range",x);
}

//====================================
void regDigRead(int pin, int value,int step)
//====================================
{
  int x;
  char temp[300];
  x = stepAtReadD[0];
  x++;
  stepAtReadD[0] = x;
  if(x < MAX_READ)
    {
      stepAtReadD[x]  = step;
      valueAtReadD[x] = value;
      pinAtReadD[x]   = pin;
      //sprintf(temp,"regDigRead: x=%d step=%d pin=%d value=%d\n",x,step,pin,value);
      //errorLog(temp);
    }
  else
    errorLog("Number of digital events in scenario out of range",x);
}


//====================================
void interruptNow()
//====================================
{
  int i,ir0_1,ir0_2,ir1_1,ir1_2;
  int ir,ir_1,ir_2,pin;
  i = timeFromStart;

  // Trigger interrupt if any
  //if(g_pinNo == currentPin && timeFromStart == g_pinStep && g_pinType == DIG)
  //  {
  //
  //  }


      //printf("interrupt now %d %d\n",i,g_pinStep);
      for(ir=0;ir<=5;ir++)
	{
	  if(attached[ir] == YES)
	    {
	      pin = inrpt[ir];
	      ir_1 = getDigitalPinValue(pin,i);
	      ir_2 = getDigitalPinValue(pin,i-1);
	      //regDigRead(pin,ir_1,i);
	      //printf("ir=%d pin %d 1=%d 2=%d\n",ir,pin,ir_1,ir_2);

	      //printf("%d check interrupt=%d pin=%d cur=%d prev=%d mode=%d\n",timeFromStart,ir,i,ir_1,ir_2,interruptMode[ir]);
	      if(interruptMode[ir] == RISING && ir_1 == 1 && ir_2 == 0)
		{
		  passTime();
		  //regDigRead(pin,ir_2,timeFromStart-1);
		  regDigRead(pin,ir_1,timeFromStart);
		  //printf("RISING interrupt=%d pin=%d cur=%d prev=%d mode=%d\n",ir,i,ir_1,ir_2,interruptMode[ir]);
		  //passTime();	
		  wLog1(interruptType[RISING],ir);
		  mLineText("interrupt in");
		  interrupt[ir]();
		  mLineText("interrupt out");
		}
	      if(interruptMode[ir] == FALLING && ir_1 == 0 && ir_2 == 1)
		{
		  passTime();
		  //regDigRead(pin,ir_2,timeFromStart-1);
		  regDigRead(pin,ir_1,timeFromStart);
		  //printf("FALLING interrupt=%d pin=%d cur=%d prev=%d mode=%d\n",ir,i,ir_1,ir_2,interruptMode[ir]);
		  //passTime();
		  wLog1(interruptType[FALLING],ir);
		  mLineText("interrupt in");
		  interrupt[ir]();
		  mLineText("interrupt out");
		}

	      if(interruptMode[ir] == CHANGE && ir_1 != ir_2)
		{
		  passTime();
		  //regDigRead(pin,ir_2,timeFromStart-1);
		  regDigRead(pin,ir_1,timeFromStart);
		  //printf("CHANGE interrupt=%d pin=%d cur=%d prev=%d mode=%d\n",ir,i,ir_1,ir_2,interruptMode[ir]);
		  //passTime();
		  wLog1(interruptType[CHANGE],ir);
		  mLineText("interrupt in");
		  interrupt[ir]();
		  mLineText("interrupt out");
		}
	    }
	} 
}
//====================================
void readScenario()
//====================================
{
  FILE *in;
  char row[120],*p,scenType[200], junk[20];
  int x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,temp;
  int i,state=0;



  if(g_scenSource == 0)in = fopen("sketch.pde","r");
  if(g_scenSource == 1)in = fopen("data.scen","r");

  if(in == NULL)
    {
      mLog0("Unable to open sketch for scenario reading\n");
    }
  else
    {
      while (fgets(row,120,in)!=NULL)
	{
	  if(p=strstr(row,"SCENDIGPIN"))
	    {
	      sscanf(p,"%s%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",junk,&temp,&x0,&x1,&x2,&x3,&x4,&x5,&x6,&x7,&x8,&x9,&x10,&x11,&x12,&x13);
	      scenDigital++;
	      i = scenDigital;
	      if(temp < s_digitalStep[i-1])
		fprintf(e_log,"Error:Scenario data not given in increasing order: Digatal Step %d\n",temp);
	      s_digitalStep[i] = temp;
	      s_digitalPin[i][0]= x0;
	      s_digitalPin[i][1]= x1;
	      s_digitalPin[i][2]= x2;
	      s_digitalPin[i][3]= x3;
	      s_digitalPin[i][4]= x4;
	      s_digitalPin[i][5]= x5;
	      s_digitalPin[i][6]= x6;
	      s_digitalPin[i][7]= x7;
	      s_digitalPin[i][8]= x8;
	      s_digitalPin[i][9]= x9;
	      s_digitalPin[i][10]= x10;
	      s_digitalPin[i][11]= x11;
	      s_digitalPin[i][12]= x12;
	      s_digitalPin[i][13]= x13;
	    }
	  if(p=strstr(row,"SCENANAPIN"))
	    {
	      sscanf(p,"%s%d%d%d%d%d%d%d",junk,&temp,&x0,&x1,&x2,&x3,&x4,&x5);
	      scenAnalog++;
	      i = scenAnalog;
	      if(temp < s_analogStep[i-1])
		fprintf(e_log,"Error:Scenario data not given in increasing order: Analog Step %d\n",temp);
	      s_analogStep[i] = temp;
	      s_analogPin[i][0]= x0;
	      s_analogPin[i][1]= x1;
	      s_analogPin[i][2]= x2;
	      s_analogPin[i][3]= x3;
	      s_analogPin[i][4]= x4;
	      s_analogPin[i][5]= x5;
	    }
	  if(p=strstr(row,"SCENINRPT"))
	    {
	      sscanf(p,"%s%d%d%d",junk,&temp,&x0,&x1);
	      scenInterrupt++;
	      i = scenInterrupt;
	      if(temp < s_interruptStep[i-1])
		fprintf(e_log,"Error:Scenario data not given in increasing order: Interrupt Step %d\n",temp);
	      s_interruptStep[i] = temp;
	      s_interrupt[i][0]  = x0;
	      s_interrupt[i][1]  = x1;
	    }
	}
    }
}

//====================================
// End of file
//====================================
