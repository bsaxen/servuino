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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 

#define TOTAL_PINS 20
#define ANAPINS 6
#define DIGPINS 14
#define INTPINS 6

#define D00  0
#define D01  1
#define D02  2
#define D03  3
#define D04  4
#define D05  5
#define D06  6
#define D07  7
#define D08  8
#define D09  9
#define D10  10
#define D11  11
#define D12  12
#define D13  13

#define A00  14
#define A01  15
#define A02  16
#define A03  17
#define A04  18
#define A05  19


#define LOW    0
#define HIGH   1
#define INPUT  1
#define OUTPUT 2
#define INTERRUPT 3

#define BYTE   1
#define BIN    2
#define OCT    3
#define DEC    4
#define HEX    5

#define ADD 10
#define DELETE 20

#define CHANGE  1
#define RISING  2
#define FALLING 3

#define ANA    1
#define DIG    2

#define ON     1
#define OFF    0

#define YES    1
#define NO     2

#define FREE   0
#define RX     3
#define TX     4

#define SCEN_MAX  2000
#define MAX_LOOPS 1000
#define LOG_MAX   200
#define LOG_TEXT_SIZE 120
#define MAX_READ 900

#define IR0  2
#define IR1  3
#define IR2 21
#define IR3 20
#define IR4 19
#define IR5 18

char sketch[120];

// Init

int currentStep = 0;
int g_simulationLength = 111;
int g_go = NO;

char interruptType[5][80];
void (*interrupt[6])();
void (*interrupt0)();
void (*interrupt1)();
void (*interrupt2)();
void (*interrupt3)();
void (*interrupt4)();
void (*interrupt5)();
int  pinToInterrupt[INTPINS];

void stepCommand();

int   row,col;
int   graph_x = 10,graph_y = 10;

char  appName[120];

int   anaPinPos[ANAPINS];
int   c_analogPin[ANAPINS];
int   s_analogPin[SCEN_MAX][ANAPINS];
int   s_analogStep[SCEN_MAX][ANAPINS];

int   digPinPos[DIGPINS];
int   c_digitalPin[DIGPINS];
int   s_digitalPin[SCEN_MAX][DIGPINS];
int   s_digitalStep[SCEN_MAX][DIGPINS];
int   digitalMode[DIGPINS];

int   intPinPos[INTPINS];
int   c_intPin[INTPINS];
int   s_interrupt[SCEN_MAX][INTPINS];
int   s_interruptStep[SCEN_MAX];
int   interruptMode[INTPINS];
int   attached[INTPINS];
int   inrpt[INTPINS]; // Interrupt No -> Pin No

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
int g_interpolation = NO;
int g_nAnalogPins = 6;
int g_nDigitalPins = 14;

int preValueA[ANAPINS];
int preValueD[DIGPINS];
int curValueA[ANAPINS];
int curValueD[DIGPINS];

int currentPin = 0;

FILE *s_log,*e_log;

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

  inrpt[0] = IR0;
  inrpt[1] = IR1;
  inrpt[2] = IR2;
  inrpt[3] = IR3;
  inrpt[4] = IR4;
  inrpt[5] = IR5;

  fprintf(s_log,"# SCENARIODATA %d %d %d\n",scenDigital,scenAnalog,scenInterrupt);
  fprintf(s_log,"# LOOP %d\n",g_nloop);
  setup();
  mLineText("Start Looping");

  for(i=0;i<MAX_LOOPS;i++)  
    {
      g_nloop++;
      fprintf(s_log,"# LOOP %d\n",g_nloop);
      loop();  
      mLineText("loop shift");
    }
  stopEncoding();
  return;
}


//====================================
int main(int argc, char *argv[])
//====================================
{
  int x;
  g_go = YES;
  openSimFile();

  boardInit();
  readScenario();

  if(argc == 1)
    {
      readSketchInfo();
      g_simulationLength = 499;
      g_scenSource = 0;
      runEncoding(g_simulationLength);
    }
  if(argc == 3)
    {
      readSketchInfo();
      g_simulationLength = atoi(argv[1]);
      g_scenSource = atoi(argv[2]);
      runEncoding(g_simulationLength);
    }
  else if(argc == 8)
    {
      readSketchInfo();
      // steps, source, pintype, pinno, pinvalue, pinstep,action
      g_simulationLength = atoi(argv[1]);
      g_scenSource =  atoi(argv[2]);
      g_pinType    =  atoi(argv[3]);
      g_pinNo      =  atoi(argv[4]);
      g_pinValue   =  atoi(argv[5]);
      g_pinStep    =  atoi(argv[6]);
      g_action     =  atoi(argv[7]);

      readScenario();// read from data.scen

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

  closeSimFile();
}
 

 
