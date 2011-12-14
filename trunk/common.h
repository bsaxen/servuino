// Board ====================

#define UNO    1
#define MEGA   2

#define ANA    1
#define DIG    2

#define MAX_PIN_IR_UNO 2
#define MAX_PIN_IR_MEGA 6

#define MAX_PIN_ANALOG_UNO 6
#define MAX_PIN_DIGITAL_UNO 14

#define MAX_PIN_ANALOG_MEGA 16
#define MAX_PIN_DIGITAL_MEGA 54

//#define TOTAL_PINS 20

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

#define IR0  2
#define IR1  3
#define IR2 21
#define IR3 20
#define IR4 19
#define IR5 18

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5

// Logical ===================
#define ON     1
#define OFF    0

#define YES    1
#define NO     0

#define HEAL  101

#define S_OK    0
#define S_NOK   1

// Values ====================
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

// Interrupt =================
#define CHANGE  11
#define RISING  12
#define FALLING 13


#define FREE   0
#define RX     23
#define TX     24

#define RUN    1
#define ADMIN  2
#define STOP   1


#define FORWARD  1
#define BACKWARD 2


// Dimension
#define SCEN_MAX  2000
#define MAX_LOOPS 1000
#define LOG_MAX   200
#define LOG_TEXT_SIZE 120
#define MAX_READ 900
#define MAX_STEP 2000
#define MAX_LOOP 2000
#define MAX_SERIAL_BUFFER 900
#define SIZE_ROW 180

// Misc =======================
#define BYTE   1
#define BIN    2
#define OCT    3
#define DEC    4
#define HEX    5



void errorLog(const char msg[], int x);
int checkRange(int mode,const char check[],int value);

int max_digVal,min_digVal,max_anaVal,min_anaVal;
int max_digPin,min_digPin,max_anaPin,min_anaPin;
int max_irPin,min_irPin;
int max_pwm,min_pwm;

int max_steps;
int max_loops;

int inrpt[MAX_PIN_IR_MEGA];
int attached[MAX_PIN_IR_MEGA];

// End of file

