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
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <math.h> 
using namespace std;

//#include "String.h"
#include "common.h"


char sketch[120];

// Init

int  currentStep = 0;
int  g_simulationLength = 111;
char g_version[40];

char interruptType[50][80];
void (*interrupt[6])();

//int  pinToInterrupt[INTPINS];

void stepCommand();

int   row,col;
int   graph_x = 10,graph_y = 10;

char  appName[120];

int   anaPinPos[MAX_PIN_ANALOG_MEGA];
int   c_analogPin[MAX_PIN_ANALOG_MEGA];
int   s_analogPin[SCEN_MAX][MAX_PIN_ANALOG_MEGA];
int   s_analogStep[SCEN_MAX][MAX_PIN_ANALOG_MEGA];

int   digPinPos[MAX_PIN_DIGITAL_MEGA];
int   c_digitalPin[MAX_PIN_DIGITAL_MEGA];
int   s_digitalPin[SCEN_MAX][MAX_PIN_DIGITAL_MEGA];
int   s_digitalStep[SCEN_MAX][MAX_PIN_DIGITAL_MEGA];
int   digitalMode[MAX_PIN_DIGITAL_MEGA];

int   s_interrupt[SCEN_MAX][MAX_PIN_IR_MEGA];
int   s_interruptStep[SCEN_MAX];
int   interruptMode[MAX_PIN_IR_MEGA];

char  textPinModeIn[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];
char  textPinModeOut[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];
char  textDigitalWriteLow[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];
char  textDigitalWriteHigh[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];
char  textAnalogWrite[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];
char  textAnalogRead[MAX_PIN_ANALOG_MEGA][SIZE_ROW];
char  textDigitalRead[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];

int   stepAtReadD[MAX_READ];
int   stepAtReadA[MAX_READ];
int   valueAtReadD[MAX_READ];
int   valueAtReadA[MAX_READ];
int   pinAtReadD[MAX_READ];
int   pinAtReadA[MAX_READ];

int   paceMaker = 0;
int   baud = 0;
int   error = 0;
int   logging = YES;
int   serialSize = 1;
int   serialMode = OFF;
int   scenAnalog    = 0;
int   scenDigital   = 0;
int   scenInterrupt = 0;

int   conn;

// Configuration default values

int   confLogLev  =   0;

int g_nloop = 0;
int g_scenSource = 0;
int g_pinType    = 0;
int g_pinNo      = 0;
int g_pinValue   = 0;
int g_pinStep    = 0;
int g_action     = 0;

int g_allowInterrupt = YES;
int g_interpolation  = NO;
//int g_nAnalogPins    = 6;
//int g_nDigitalPins   = 14;

int boardType = UNO;
int nCodeString = 0;

//int preValueA[MAX_PIN_ANALOG_MEGA];
//int preValueD[DIGPINS];
//int curValueA[MAX_PIN_ANALOG_MEGA];
//int curValueD[DIGPINS];

int currentPin = 0;

FILE *s_log,*e_log,*c_log,*a_log,*u_log,*x_log,*t_log;

#include "code.h"
#include "common_lib.c"
#include "servuino.h"
#include "servuino_lib.c"
#include "arduino_lib.c"
#include "sketch.pde"

//====================================
void runEncoding(int n)
//====================================
{
  int i;
  strcpy(interruptType[LOW],"interruptLOW");
  strcpy(interruptType[FALLING],"interruptFALLING");
  strcpy(interruptType[RISING],"interruptRISING");
  strcpy(interruptType[CHANGE],"interruptCHANGE");

  //fprintf(s_log,"# SCENARIODATA %d %d %d\n",scenDigital,scenAnalog,scenInterrupt);
  //fprintf(s_log,"# LOOP %d\n",g_nloop);

  passTime();
  wLog0(0,"Setup");
  wLog0(1,"Setup");
  setup();

  for(i=0;i<MAX_LOOPS;i++)  
    {
      g_nloop++;
      passTime();
      codeLog(F_LOOP,g_nloop,0,0,0,NULL);
      wLog1(0,"servuinoLoop",g_nloop);
      wLog1(1,"Loop",g_nloop);
      loop();  
    }
  stopEncoding();
  return;
}


//====================================
int main(int argc, char *argv[])
//====================================
{
  int x;

  strcpy(g_version,"0.0.4");

  openFiles();
  readSketchInfo();
  setRange(boardType);
 
  boardInit();
  readScenario();
  readCustomLog();

  if(argc == 1)
    {
      g_simulationLength = 901;
      g_scenSource = 0;
      runEncoding(g_simulationLength);
    }
  if(argc == 3)
    {
      g_simulationLength = atoi(argv[1]);
      g_scenSource = atoi(argv[2]);
      readScenario(); // Maybe other source
      runEncoding(g_simulationLength);

    }
  else if(argc == 8)
    {
      // steps, source, pintype, pinno, pinvalue, pinstep,action
      g_simulationLength = atoi(argv[1]);
      g_scenSource =  atoi(argv[2]);
      g_pinType    =  atoi(argv[3]);
      g_pinNo      =  atoi(argv[4]);
      g_pinValue   =  atoi(argv[5]);
      g_pinStep    =  atoi(argv[6]);
      g_action     =  atoi(argv[7]);

      readScenario();// read from data.scen

      max_steps = g_simulationLength;

      if(g_pinType == DIG)
	{ 
	  if(g_action == ADD)x = insDigitalPinValue(g_pinNo,g_pinStep,g_pinValue);
	  if(g_action == DELETE)x = delDigitalPinValue(g_pinNo,g_pinStep);

	}
      if(g_pinType == ANA)
	{ 
	  if(g_action == ADD)x = insAnalogPinValue(g_pinNo,g_pinStep,g_pinValue);
	  if(g_action == DELETE)x = delAnalogPinValue(g_pinNo,g_pinStep);

	}

      runEncoding(g_simulationLength);
    }
  else
    errorLog("Servuino not executed",0);

  closeFiles();
}
 

 
