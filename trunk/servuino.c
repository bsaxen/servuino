//================================================
//  Servuino 
//  Developed by Benny Saxen, ADCAJO
//================================================
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

#define DP 5
#define AP 15
#define RF 0
#define ER 1
#define SR 20

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
  readSketchInfo();
  if(argc == 2) g_simulationLength = atoi(argv[1]);
  runEncoding(g_simulationLength);
  closeSimFile();

}


 
