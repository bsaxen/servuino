//================================================
//  Developed by Benny Saxen, ADCAJO
//================================================


#define FREE   0
#define RX     3
#define TX     4

#define SCEN_MAX  100
#define LOG_MAX   200
#define MEM_MAX   2
#define LOG_TEXT_SIZE 120

int   row,col;
int   graph_x = 10,graph_y = 10;
int   digPinPos[14];
int   anaPinPos[6];
char  appName[80];

int   c_analogPin[MEM_MAX][6];
int   c_digitalPin[MEM_MAX][14];

int   s_analogPin[SCEN_MAX][6];
int   s_digitalPin[SCEN_MAX][14];
int   s_interrupt[SCEN_MAX][2];
int   s_analogStep[SCEN_MAX];
int   s_digitalStep[SCEN_MAX];
int   s_interruptStep[SCEN_MAX];

int   interruptMode[2];
int   digitalMode[100];
int   paceMaker = 0;
int   baud = 0;
int   error = 0;
int   logging = YES;
char  logBuffer[LOG_MAX][100];
int   logSize = 1;
int   serialSize = 1;
int   serialMode = OFF;
char  serialBuffer[100][100];
int   rememberNewLine;
char  textPinModeIn[14][LOG_TEXT_SIZE];
char  textPinModeOut[14][LOG_TEXT_SIZE];
char  textDigitalWriteLow[14][LOG_TEXT_SIZE];
char  textDigitalWriteHigh[14][LOG_TEXT_SIZE];
char  textAnalogWrite[14][LOG_TEXT_SIZE];
char  textAnalogRead[14][LOG_TEXT_SIZE];
char  textDigitalRead[14][LOG_TEXT_SIZE];
int   scenAnalog    = 0;
int   scenDigital   = 0;
int   scenInterrupt = 0;

int   conn;

// Configuration default values
int   confDelay   = 100;
int   confLogLev  =   1;
int   confLogFile =   0;


int g_now  = 0;
int g_prev = 1;
int g_nloop = 0;

//====================================
void boardInit()
//====================================
{
  int i,j;

  g_nloop = 0;
  for(i=0;i<14;i++)
    {
      digitalMode[i] = FREE;
      strcpy(textPinModeIn[i],"void");
      strcpy(textPinModeOut[i],"void");

      strcpy(textDigitalWriteLow[i],"void");
      strcpy(textDigitalWriteHigh[i],"void");

      strcpy(textAnalogWrite[i],"void");
      strcpy(textAnalogRead[i],"void");

      strcpy(textDigitalRead[i],"void");
    }
  for(i=0;i<14;i++)
    {
      for(j=0;j<MEM_MAX;j++)
	{
          s_analogPin[j][i]   = 0;
          s_digitalPin[j][i]  = 0;
	}
    }
}

//====================================
void showError(const char *m, int value)
//====================================
{
  error = 1;

  if(value == -1)
    fprintf(s_log,"# ERROR %s",m);
  else
    fprintf(s_log,"# ERROR %s %d",m,value);
}

//====================================
void openSimFile()
//====================================
{
  s_log = fopen("data.su","w");
  if(s_log == NULL)
    {
      showError("Unable to open data.su",-1);
    }
}

//====================================
void closeSimFile()
//====================================
{
  fclose(s_log);
}


//====================================
int getAnalogPinValue(int pin,int step)
//====================================
{  
  int i,res=0;

  for (i=0;i<scenAnalog;i++)
  {
    if(step > s_analogStep[i] && step < s_analogStep[i+1])
       res = s_analogPin[i][pin];
  }
  if(step > s_analogStep[scenAnalog]) res = s_analogPin[scenAnalog][pin];

  return(res);
}  

//====================================
int getDigitalPinValue(int pin,int step)
//====================================
{  
  int i,res=0;
  for (i=0;i<scenDigital;i++)
  {
    if(step > s_digitalStep[i] && step < s_digitalStep[i+1])
       res = s_digitalPin[i][pin];
  }
  if(step > s_digitalStep[scenDigital]) res = s_digitalPin[scenDigital][pin];

  return(res);
}  

//====================================
int getInterruptValue(int pin,int step)
//====================================
{  
  int i,res=0;
  for (i=0;i<scenInterrupt;i++)
  {
    if(step > s_interruptStep[i] && step < s_interruptStep[i+1])
       res = s_interrupt[i][pin];
  }
  if(step > s_interruptStep[scenInterrupt]) res = s_interrupt[scenInterrupt][pin];

  return(res);
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
  for(i=0;i<13;i++)fprintf(s_log,"%3d ",c_digitalPin[g_now][i]);
  fprintf(s_log,"\n");


  fprintf(s_log,"# ANA_PIN_VAL: ");
  for(i=0;i<6;i++) fprintf(s_log,"%3d ",c_analogPin[g_now][i]);
  fprintf(s_log,"\n");

  return;
}

//====================================
void wLog(const char *p, int value1, int value2)
//====================================
{
  if(value1 == -1 && value2 == -1)
    fprintf(s_log,"%3d %s\n",timeFromStart,p);

  if(value1 > -1 && value2 == -1)
    fprintf(s_log,"%3d %s %d\n",timeFromStart,p,value1);

  if(value1 == -1 && value2 > -1)
    fprintf(s_log,"%3d %s %d\n",timeFromStart,p,value2);

  if(value1 > -1 && value2 > -1)
    fprintf(s_log,"%3d %s %d %d\n",timeFromStart,p,value1,value2);


  return;
}

//====================================
void wLogChar(const char *p, const char *value1, int value2)
//====================================
{

  if(value2 == -1)
    fprintf(s_log,"%3d %s '%s'\n",timeFromStart,p,value1);

  if(value2 > -1)
    fprintf(s_log,"%3d %s '%s' %d\n",timeFromStart,p,value1,value2);

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
      printf("%s\n",m);
      wLog(m,-1,-1);
    }
  else
    {
      showError("Serial output without Serial.begin",timeFromStart);
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
      printf("Error: Unable to open sketch",-1);
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
	      if(p=strstr(row,"PINMODE_IN:"))
		{
		  fprintf(s_log,"#%s",row);
		  pin = wCustomLog(p,res);
		  strcpy(textPinModeIn[pin],res);
		}
	      if(p=strstr(row,"PINMODE_OUT:"))
		{
		  fprintf(s_log,"#%s",row);
		  pin = wCustomLog(p,res);
		  strcpy(textPinModeOut[pin],res);
		}
	      if(p=strstr(row,"DIGITALWRITE_LOW:"))
		{
		  fprintf(s_log,"#%s",row);
		  pin = wCustomLog(p,res);
		  strcpy(textDigitalWriteLow[pin],res);
		}
	      if(p=strstr(row,"DIGITALWRITE_HIGH:"))
		{
		  fprintf(s_log,"#%s",row);
		  pin = wCustomLog(p,res);
		  strcpy(textDigitalWriteHigh[pin],res);
		}
	      if(p=strstr(row,"ANALOGREAD:"))
		{
		  fprintf(s_log,"#%s",row);
		  pin = wCustomLog(p,res);
		  strcpy(textAnalogRead[pin],res);
		}
	      if(p=strstr(row,"DIGITALREAD:"))
		{
		  fprintf(s_log,"#%s",row);
		  pin = wCustomLog(p,res);
		  strcpy(textDigitalRead[pin],res);
		}
	      if(p=strstr(row,"ANALOGWRITE:"))
		{
		  fprintf(s_log,"#%s",row);
		  pin = wCustomLog(p,res);
		  strcpy(textAnalogWrite[pin],res);
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
  status();
  closeSimFile();
  printf("Ready! Servuino Encoding in file: data.su\n");
  exit(0);
}

//====================================
void passTime()
//====================================
{
  int i,ir0_1,ir0_2,ir1_1,ir1_2;


  timeFromStart++;

  if(g_simulationLength < timeFromStart)stopEncoding();

  i = timeFromStart;

  ir0_1 = getInterruptValue(0,i);
  ir0_2 = getInterruptValue(0,i-1);
  ir1_1 = getInterruptValue(1,i);
  ir1_2 = getInterruptValue(1,i-1);

  //  if(interruptMode[0] == LOW && interrupt[i][0] == 0)
  //    {
  //      if(confLogLev > 0)wLog("InterruptLOW",0,-1);
  //      interrupt0();
  //    }


  if(interruptMode[0] == RISING && ir0_1 == 1 && ir0_2 == 0)
    {
      if(confLogLev > 0)wLog("InterruptRISING",0,-1);
      interrupt0();
    }
  
  if(interruptMode[0] == FALLING && ir0_1 == 0 && ir0_2 == 1)
    {
      if(confLogLev > 0)wLog("InterruptFALLING",0,-1);
      interrupt0();
    }

  if(interruptMode[0] == CHANGE && ir0_1 != ir0_2)
    {
      if(confLogLev > 0)wLog("InterruptCHANGE",0,-1);
      interrupt0();
    }


  if(interruptMode[1] == RISING && ir1_1 == 1 && ir1_2 == 0)
    {
      if(confLogLev > 0)wLog("InterruptRISING",1,-1);
      interrupt1();
    }
  
  if(interruptMode[1] == FALLING && ir1_1 == 0 && ir1_2 == 1)
    {
      if(confLogLev > 0)wLog("InterruptFALLING",1,-1);
      interrupt1();
    }

  if(interruptMode[1] == CHANGE && ir1_1 != ir1_2)
    {
      if(confLogLev > 0)wLog("InterruptCHANGE",1,-1);
      interrupt1();
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



  in = fopen("sketch.pde","r");
  if(in == NULL)
    {
      showError("Unable to open sketch for scenario reading",-1);
    }
  else
    {
      while (fgets(row,80,in)!=NULL)
	{
	  
	  if(p=strstr(row,"SCENDIGPIN"))
	    {
	      sscanf(p,"%s%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",junk,&temp,&x0,&x1,&x2,&x3,&x4,&x5,&x6,&x7,&x8,&x9,&x9,&x10,&x11,&x12,&x13);
	      scenDigital++;
	      i = scenDigital;
	      if(temp < s_digitalStep[i-1])
		printf("Error:Scenario data not given in increasing order: Digatal Step %d\n",temp);
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
		printf("Error:Scenario data not given in increasing order: Analog Step %d\n",temp);
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
		printf("Error:Scenario data not given in increasing order: Interrupt Step %d\n",temp);
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
