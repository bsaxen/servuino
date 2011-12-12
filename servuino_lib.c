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

  for(i=0;i<=max_anaPin;i++)
    {
      anaPinPos[i]   = 0;
      c_analogPin[i] = 0;
      for(j=0;j<SCEN_MAX;j++)
	{
	  s_analogPin[j][i]  = 0;
	  s_analogStep[j][i] = 0;
	} 
    }
  
  for(i=0;i<=max_digPin;i++)
    {
      digitalMode[i]  = FREE;
      digPinPos[i]    = 0;
      c_digitalPin[i] = 0;
      for(j=0;j<SCEN_MAX;j++)
	{
	  s_digitalPin[j][i]  = 0;
	  s_digitalStep[j][i] = 0;
	}
    }

  for(i=0;i<max_irPin;i++)
    {
      interruptMode[i] = 0;
      intPinPos[i]     = 0;
      c_intPin[i]      = 0;
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
  ok = ok + checkRange(S_OK,"digval",pin);

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
  ok = ok + checkRange(S_OK,"anaval",pin);

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
void saveScenario()
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
void iLog1(const char *p, int value1)
//====================================
{
  fprintf(s_log,"* %d %s %d\n",currentStep,p,value1);
  return;
}

//====================================
void mLine()
//====================================
{
  char line[120];
  
  strcpy(line,"--------------------");
  fprintf(s_log,"= %d %s\n",currentStep,line);
  return;
}
//====================================
void mLineText(const char *t)
//====================================
{
  char line[120];
  
  sprintf(line,"------ %s ------",t);
  fprintf(s_log,"= %d %s\n",currentStep,line);
  return;
}
//====================================
void mLog0(const char *p)
//====================================
{
  fprintf(s_log,"= %d %s\n",currentStep,p);
  return;
}
//====================================
void mLog1(const char *p, int value1)
//====================================
{
  fprintf(s_log,"= %d %s %d\n",currentStep,p,value1);
  return;
}

//====================================
void wLog0(const char *p)
//====================================
{
  fprintf(s_log,"+ %d %s\n",currentStep,p);
  return;
}

//====================================
void wLog1(const char *p, int value1)
//====================================
{
  fprintf(s_log,"+ %d %s %d\n",currentStep,p,value1);
  return;
}

//====================================
void wLog2(const char *p, int value1, int value2)
//====================================
{
  fprintf(s_log,"+ %d %s %d %d\n",currentStep,p,value1,value2);
  return;
}


//====================================
void wLogChar1(const char *p, const char *value1)
//====================================
{
  fprintf(s_log,"+ %d %s '%s'\n",currentStep,p,value1);
  return;
}

//====================================
void wLogChar2(const char *p, const char *value1, int value2)
//====================================
{
  fprintf(s_log,"+ %d %s '%s' %d\n",currentStep,p,value1,value2);
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
		  fprintf(s_log,"#%s",row);
		  q = strstr(p,":");q++;
		  sscanf(q,"%s",appName);
		}
	      if(p=strstr(row,"BOARD_TYPE"))
		{
		  fprintf(s_log,"#%s",row);
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
  fprintf(s_log,"# ENDOFSIM\n");
  //status();
  saveScenario();
  closeSimFile();
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
  int pin,step,value,i;
  int tmp=0,dCount[MAX_PIN_DIGITAL_MEGA],aCount[MAX_PIN_ANALOG_MEGA];

  if(g_scenSource == 0)in = fopen("sketch.pde","r");
  if(g_scenSource == 1)in = fopen("data.scen","r");

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
		fprintf(e_log,"Error:Scenario data not given in increasing order: Digatal Step %d\n",step);
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
		fprintf(e_log,"Error:Scenario data not given in increasing order: Analog Step %d\n",step);
	      s_analogStep[tmp][pin]   = step;
	      s_analogPin[tmp][pin]    = value;
	      s_analogStep[0][pin]     = tmp;
	    }
	}
    }
}

//====================================
// End of file
//====================================
