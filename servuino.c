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
#define INTPINS 2

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

#define BYTE   1
#define BIN    2
#define OCT    3
#define DEC    4
#define HEX    5

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

#define SCEN_MAX  100
#define LOG_MAX   200
#define LOG_TEXT_SIZE 120
#define MAX_READ 100

char sketch[120];

// Init

int timeFromStart = 0;
int g_simulationLength = 111;
int g_go = NO;

void (*interrupt0)();
void (*interrupt1)();

void stepCommand();

int   row,col;
int   graph_x = 10,graph_y = 10;

char  appName[120];

int   anaPinPos[ANAPINS];
int   c_analogPin[ANAPINS];
int   s_analogPin[SCEN_MAX][ANAPINS];
int   s_analogStep[SCEN_MAX];

int   digPinPos[DIGPINS];
int   c_digitalPin[DIGPINS];
int   s_digitalPin[SCEN_MAX][DIGPINS];
int   s_digitalStep[SCEN_MAX];
int   digitalMode[DIGPINS];

int   intPinPos[INTPINS];
int   c_intPin[INTPINS];
int   s_interrupt[SCEN_MAX][INTPINS];
int   s_interruptStep[SCEN_MAX];
int   interruptMode[INTPINS];
int   attached[INTPINS];

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

FILE *s_log,*e_log;

#include "servuino_lib.c"
#include "arduino_lib.c"
#include "sketch.pde"

//====================================
void runEncoding(int n)
//====================================
{
  boardInit();
  readScenario();

  fprintf(s_log,"# SCENARIODATA %d %d %d\n",scenDigital,scenAnalog,scenInterrupt);
  scenario();
  status();
  fprintf(s_log,"# LOOP %d\n",g_nloop);
  setup();
  mLineText("Start Looping");
  while(1)
    {
      g_nloop++;
      status();
      fprintf(s_log,"# LOOP %d\n",g_nloop);
      loop();  
      mLineText("loop shift");
    }
  return;
}


//====================================
int main(int argc, char *argv[])
//====================================
{
  g_go = YES;
  openSimFile();
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
  else if(argc == 7)
    {
      readSketchInfo();
      // steps, source, pintype, pinno, pinvalue, pinstep
      g_simulationLength = atoi(argv[1]);
      g_scenSource =  atoi(argv[2]);
      g_pinType    =  atoi(argv[3]);
      g_pinNo      =  atoi(argv[4]);
      g_pinValue   =  atoi(argv[5]);
      g_pinStep    =  atoi(argv[6]);
      runEncoding(g_simulationLength);
    }
  else
    errorLog("Servuino not executed");

  closeSimFile();
}


 
