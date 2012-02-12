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

// Math function min and max
#ifndef max
#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

#define _BV(bit) (1 << (bit))
  
char  stemp[80];

typedef int byte;
typedef int word;
typedef bool boolean;

typedef unsigned int uint8_t;
typedef unsigned int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned int uint64_t;


unsigned int B00000000=0b00000000; // 0
unsigned int B00000001=0b00000001; // 1
unsigned int B00000010=0b00000010; // 2
unsigned int B00000011=0b00000011; // 3
unsigned int B00000100=0b00000100; // 4
unsigned int B00000101=0b00000101; // 5
unsigned int B00000110=0b00000110; // 6
unsigned int B00000111=0b00000111; // 7
unsigned int B00001000=0b00001000; // 8
unsigned int B00001001=0b00001001; // 9
unsigned int B00001010=0b00001010; // 10
unsigned int B00001011=0b00001011; // 11
unsigned int B00001100=0b00001100; // 12
unsigned int B00001101=0b00001101; // 13
unsigned int B00001110=0b00001110; // 14
unsigned int B00001111=0b00001111; // 15
unsigned int B00010000=0b00010000; // 16
unsigned int B00010001=0b00010001; // 17
unsigned int B00010010=0b00010010; // 18
unsigned int B00010011=0b00010011; // 19
unsigned int B00010100=0b00010100; // 20
unsigned int B00010101=0b00010101; // 21
unsigned int B00010110=0b00010110; // 22
unsigned int B00010111=0b00010111; // 23
unsigned int B00011000=0b00011000; // 24
unsigned int B00011001=0b00011001; // 25
unsigned int B00011010=0b00011010; // 26
unsigned int B00011011=0b00011011; // 27
unsigned int B00011100=0b00011100; // 28
unsigned int B00011101=0b00011101; // 29
unsigned int B00011110=0b00011110; // 30
unsigned int B00011111=0b00011111; // 31
unsigned int B00100000=0b00100000; // 32
unsigned int B00100001=0b00100001; // 33
unsigned int B00100010=0b00100010; // 34
unsigned int B00100011=0b00100011; // 35
unsigned int B00100100=0b00100100; // 36
unsigned int B00100101=0b00100101; // 37
unsigned int B00100110=0b00100110; // 38
unsigned int B00100111=0b00100111; // 39
unsigned int B00101000=0b00101000; // 40
unsigned int B00101001=0b00101001; // 41
unsigned int B00101010=0b00101010; // 42
unsigned int B00101011=0b00101011; // 43
unsigned int B00101100=0b00101100; // 44
unsigned int B00101101=0b00101101; // 45
unsigned int B00101110=0b00101110; // 46
unsigned int B00101111=0b00101111; // 47
unsigned int B00110000=0b00110000; // 48
unsigned int B00110001=0b00110001; // 49
unsigned int B00110010=0b00110010; // 50
unsigned int B00110011=0b00110011; // 51
unsigned int B00110100=0b00110100; // 52
unsigned int B00110101=0b00110101; // 53
unsigned int B00110110=0b00110110; // 54
unsigned int B00110111=0b00110111; // 55
unsigned int B00111000=0b00111000; // 56
unsigned int B00111001=0b00111001; // 57
unsigned int B00111010=0b00111010; // 58
unsigned int B00111011=0b00111011; // 59
unsigned int B00111100=0b00111100; // 60
unsigned int B00111101=0b00111101; // 61
unsigned int B00111110=0b00111110; // 62
unsigned int B00111111=0b00111111; // 63
unsigned int B01000000=0b01000000; // 64
unsigned int B01000001=0b01000001; // 65
unsigned int B01000010=0b01000010; // 66
unsigned int B01000011=0b01000011; // 67
unsigned int B01000100=0b01000100; // 68
unsigned int B01000101=0b01000101; // 69
unsigned int B01000110=0b01000110; // 70
unsigned int B01000111=0b01000111; // 71
unsigned int B01001000=0b01001000; // 72
unsigned int B01001001=0b01001001; // 73
unsigned int B01001010=0b01001010; // 74
unsigned int B01001011=0b01001011; // 75
unsigned int B01001100=0b01001100; // 76
unsigned int B01001101=0b01001101; // 77
unsigned int B01001110=0b01001110; // 78
unsigned int B01001111=0b01001111; // 79
unsigned int B01010000=0b01010000; // 80
unsigned int B01010001=0b01010001; // 81
unsigned int B01010010=0b01010010; // 82
unsigned int B01010011=0b01010011; // 83
unsigned int B01010100=0b01010100; // 84
unsigned int B01010101=0b01010101; // 85
unsigned int B01010110=0b01010110; // 86
unsigned int B01010111=0b01010111; // 87
unsigned int B01011000=0b01011000; // 88
unsigned int B01011001=0b01011001; // 89
unsigned int B01011010=0b01011010; // 90
unsigned int B01011011=0b01011011; // 91
unsigned int B01011100=0b01011100; // 92
unsigned int B01011101=0b01011101; // 93
unsigned int B01011110=0b01011110; // 94
unsigned int B01011111=0b01011111; // 95
unsigned int B01100000=0b01100000; // 96
unsigned int B01100001=0b01100001; // 97
unsigned int B01100010=0b01100010; // 98
unsigned int B01100011=0b01100011; // 99
unsigned int B01100100=0b01100100; // 100
unsigned int B01100101=0b01100101; // 101
unsigned int B01100110=0b01100110; // 102
unsigned int B01100111=0b01100111; // 103
unsigned int B01101000=0b01101000; // 104
unsigned int B01101001=0b01101001; // 105
unsigned int B01101010=0b01101010; // 106
unsigned int B01101011=0b01101011; // 107
unsigned int B01101100=0b01101100; // 108
unsigned int B01101101=0b01101101; // 109
unsigned int B01101110=0b01101110; // 110
unsigned int B01101111=0b01101111; // 111
unsigned int B01110000=0b01110000; // 112
unsigned int B01110001=0b01110001; // 113
unsigned int B01110010=0b01110010; // 114
unsigned int B01110011=0b01110011; // 115
unsigned int B01110100=0b01110100; // 116
unsigned int B01110101=0b01110101; // 117
unsigned int B01110110=0b01110110; // 118
unsigned int B01110111=0b01110111; // 119
unsigned int B01111000=0b01111000; // 120
unsigned int B01111001=0b01111001; // 121
unsigned int B01111010=0b01111010; // 122
unsigned int B01111011=0b01111011; // 123
unsigned int B01111100=0b01111100; // 124
unsigned int B01111101=0b01111101; // 125
unsigned int B01111110=0b01111110; // 126
unsigned int B01111111=0b01111111; // 127
unsigned int B10000000=0b10000000; // 128
unsigned int B10000001=0b10000001; // 129
unsigned int B10000010=0b10000010; // 130
unsigned int B10000011=0b10000011; // 131
unsigned int B10000100=0b10000100; // 132
unsigned int B10000101=0b10000101; // 133
unsigned int B10000110=0b10000110; // 134
unsigned int B10000111=0b10000111; // 135
unsigned int B10001000=0b10001000; // 136
unsigned int B10001001=0b10001001; // 137
unsigned int B10001010=0b10001010; // 138
unsigned int B10001011=0b10001011; // 139
unsigned int B10001100=0b10001100; // 140
unsigned int B10001101=0b10001101; // 141
unsigned int B10001110=0b10001110; // 142
unsigned int B10001111=0b10001111; // 143
unsigned int B10010000=0b10010000; // 144
unsigned int B10010001=0b10010001; // 145
unsigned int B10010010=0b10010010; // 146
unsigned int B10010011=0b10010011; // 147
unsigned int B10010100=0b10010100; // 148
unsigned int B10010101=0b10010101; // 149
unsigned int B10010110=0b10010110; // 150
unsigned int B10010111=0b10010111; // 151
unsigned int B10011000=0b10011000; // 152
unsigned int B10011001=0b10011001; // 153
unsigned int B10011010=0b10011010; // 154
unsigned int B10011011=0b10011011; // 155
unsigned int B10011100=0b10011100; // 156
unsigned int B10011101=0b10011101; // 157
unsigned int B10011110=0b10011110; // 158
unsigned int B10011111=0b10011111; // 159
unsigned int B10100000=0b10100000; // 160
unsigned int B10100001=0b10100001; // 161
unsigned int B10100010=0b10100010; // 162
unsigned int B10100011=0b10100011; // 163
unsigned int B10100100=0b10100100; // 164
unsigned int B10100101=0b10100101; // 165
unsigned int B10100110=0b10100110; // 166
unsigned int B10100111=0b10100111; // 167
unsigned int B10101000=0b10101000; // 168
unsigned int B10101001=0b10101001; // 169
unsigned int B10101010=0b10101010; // 170
unsigned int B10101011=0b10101011; // 171
unsigned int B10101100=0b10101100; // 172
unsigned int B10101101=0b10101101; // 173
unsigned int B10101110=0b10101110; // 174
unsigned int B10101111=0b10101111; // 175
unsigned int B10110000=0b10110000; // 176
unsigned int B10110001=0b10110001; // 177
unsigned int B10110010=0b10110010; // 178
unsigned int B10110011=0b10110011; // 179
unsigned int B10110100=0b10110100; // 180
unsigned int B10110101=0b10110101; // 181
unsigned int B10110110=0b10110110; // 182
unsigned int B10110111=0b10110111; // 183
unsigned int B10111000=0b10111000; // 184
unsigned int B10111001=0b10111001; // 185
unsigned int B10111010=0b10111010; // 186
unsigned int B10111011=0b10111011; // 187
unsigned int B10111100=0b10111100; // 188
unsigned int B10111101=0b10111101; // 189
unsigned int B10111110=0b10111110; // 190
unsigned int B10111111=0b10111111; // 191
unsigned int B11000000=0b11000000; // 192
unsigned int B11000001=0b11000001; // 193
unsigned int B11000010=0b11000010; // 194
unsigned int B11000011=0b11000011; // 195
unsigned int B11000100=0b11000100; // 196
unsigned int B11000101=0b11000101; // 197
unsigned int B11000110=0b11000110; // 198
unsigned int B11000111=0b11000111; // 199
unsigned int B11001000=0b11001000; // 200
unsigned int B11001001=0b11001001; // 201
unsigned int B11001010=0b11001010; // 202
unsigned int B11001011=0b11001011; // 203
unsigned int B11001100=0b11001100; // 204
unsigned int B11001101=0b11001101; // 205
unsigned int B11001110=0b11001110; // 206
unsigned int B11001111=0b11001111; // 207
unsigned int B11010000=0b11010000; // 208
unsigned int B11010001=0b11010001; // 209
unsigned int B11010010=0b11010010; // 210
unsigned int B11010011=0b11010011; // 211
unsigned int B11010100=0b11010100; // 212
unsigned int B11010101=0b11010101; // 213
unsigned int B11010110=0b11010110; // 214
unsigned int B11010111=0b11010111; // 215
unsigned int B11011000=0b11011000; // 216
unsigned int B11011001=0b11011001; // 217
unsigned int B11011010=0b11011010; // 218
unsigned int B11011011=0b11011011; // 219
unsigned int B11011100=0b11011100; // 220
unsigned int B11011101=0b11011101; // 221
unsigned int B11011110=0b11011110; // 222
unsigned int B11011111=0b11011111; // 223
unsigned int B11100000=0b11100000; // 224
unsigned int B11100001=0b11100001; // 225
unsigned int B11100010=0b11100010; // 226
unsigned int B11100011=0b11100011; // 227
unsigned int B11100100=0b11100100; // 228
unsigned int B11100101=0b11100101; // 229
unsigned int B11100110=0b11100110; // 230
unsigned int B11100111=0b11100111; // 231
unsigned int B11101000=0b11101000; // 232
unsigned int B11101001=0b11101001; // 233
unsigned int B11101010=0b11101010; // 234
unsigned int B11101011=0b11101011; // 235
unsigned int B11101100=0b11101100; // 236
unsigned int B11101101=0b11101101; // 237
unsigned int B11101110=0b11101110; // 238
unsigned int B11101111=0b11101111; // 239
unsigned int B11110000=0b11110000; // 240
unsigned int B11110001=0b11110001; // 241
unsigned int B11110010=0b11110010; // 242
unsigned int B11110011=0b11110011; // 243
unsigned int B11110100=0b11110100; // 244
unsigned int B11110101=0b11110101; // 245
unsigned int B11110110=0b11110110; // 246
unsigned int B11110111=0b11110111; // 247
unsigned int B11111000=0b11111000; // 248
unsigned int B11111001=0b11111001; // 249
unsigned int B11111010=0b11111010; // 250
unsigned int B11111011=0b11111011; // 251
unsigned int B11111100=0b11111100; // 252
unsigned int B11111101=0b11111101; // 253
unsigned int B11111110=0b11111110; // 254
unsigned int B11111111=0b11111111; // 255


//=====================================
// Functions
//=====================================


//------ Digital I/O -----------------------

void pinMode(int pin,int mode)
{
  if(mode == INPUT)
    servuinoFunc(S_PIN_MODE_INPUT,pin,mode,NULL);
  if(mode == OUTPUT)
    servuinoFunc(S_PIN_MODE_OUTPUT,pin,mode,NULL);
  return;
}
void digitalWrite(int pin,int value)
{
  if(value == 0)
    servuinoFunc(S_DIGITAL_WRITE_LOW,pin,value,NULL);
  if(value == 1)
    servuinoFunc(S_DIGITAL_WRITE_HIGH,pin,value,NULL);
  return;
}
int digitalRead(int pin)
{
  return(servuinoFunc(S_DIGITAL_READ,pin,0,NULL));
}

void analogWrite(int pin,int value) //PWM
{
  servuinoFunc(S_ANALOG_WRITE,pin,value,NULL);
  return;
}

int analogRead(int pin)
{
  pin = g_nDigPins + pin;
  return(servuinoFunc(S_ANALOG_READ,pin,0,NULL));
}

//------ Advanced I/O ----------------------
void tone(int pin, unsigned int freq)
{
  servuinoFunc(S_UNIMPLEMENTED,0,0,"tone()");
}

void tone(int pin, unsigned int freq, unsigned long duration)
{
  servuinoFunc(S_UNIMPLEMENTED,0,0,"tone()");
}

void noTone(int pin)
{
  servuinoFunc(S_UNIMPLEMENTED,0,0,"noTone()");
}

void shiftOut(int dataPin, int clockPin, int bitOrder, int value)
{
  //bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST.
  servuinoFunc(S_UNIMPLEMENTED,0,0,"shiftOut()");
}

int shiftIn(int dataPin, int clockPin, int bitOrder)
{
  //bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST.
  servuinoFunc(S_UNIMPLEMENTED,0,0,"shiftIn()");
}

unsigned long pulseIn(int pin, int value)
{
  servuinoFunc(S_UNIMPLEMENTED,0,0,"pulseIn()");
}

unsigned long pulseIn(int pin, int value, unsigned long timeout)
{
  servuinoFunc(S_UNIMPLEMENTED,0,0,"pulseIn()");
}

//------ Time ------------------------------

unsigned long millis()
{
  return(g_curStep*100);
}

unsigned long micros()
{
  return(g_curStep*100000);
}

void delay(int ms)
{
  servuinoFunc(S_DELAY,ms,0,NULL);
  return;
}

void delayMicroseconds(int us)
{
  servuinoFunc(S_DELAY_MS,us,0,NULL);
  return;
}

//------ Math ------------------------------
void test_math()
{
  double r,x,z;
  int y;
  y = min(1,2);
  y = max(1,2);
  y = abs(1);
  r = pow(x,z);
  r = sqrt(y);
}


double sq(double x)
{
  return(sqrt(x));
}

int map(int x, int fromLow, int fromHigh, int toLow, int toHigh)
{
  int y;
  y = (float)(x-fromLow)/(fromHigh - fromLow)*(toHigh - toLow) + toLow;
  //printf("%d %d\n",x,y);
  return(y);
}

int constrain(int x, int a, int b)
{
  int res;
  if(x<=b && x>=a)res = x;
  if(x>b)res = b;
  if(x<a)res = a;
  return(res);
}

//------ Trigonometry ----------------------
void test_trigonometry()
{
  double x;
  x = sin(1);
  x = cos(1);
  x = tan(1);
}
//------ Random Numbers --------------------
void randomSeed(int seed)
{
  srand(seed);
}

long random(long upperLimit)
{
  long x = RAND_MAX/upperLimit;
  x = long(rand()/x);
  return(x);
}

long random(long lowerLimit, long upperLimit)
{
  long interval, temp=0;
  if (lowerLimit<upperLimit) 
    { 
      interval = upperLimit - lowerLimit;
      temp = lowerLimit + random(interval);
    }
  return(temp);
}


//------ Bits and Bytes --------------------
unsigned char lowByte(word x)
{
  return(x&0xff);
}

unsigned char highByte(word x)
{
  unsigned char y;
  x = x&0xff00;
  y = x >> 8;
  return(y);
}

int bitRead(int x, int n)
{
  int bit;
  //printf("bitRead: x=%d n=%d PORTB=%d\n",x,n,PORTB);
  bit = x >> n;
  bit = bit&0x0001;
  //printf("bitRead: x=%d n=%d PORTB=%d bit=%d\n",x,n,PORTB,bit);
  return(bit);
}

void  bitSet(unsigned int *x, int n)
{
  int res,mask;

  mask = 1 << n;
  *x  = *x | mask;

  return     ;
}

void bitClear(unsigned int *x, int n)
{
  int res,mask;

  mask = 1 << n;
  *x  = *x & ~mask;

  return;
}

void bitWrite(unsigned int *x, int n, int b)
{
  //printf("bitWrite: %d %d %d PORTB=%d\n",*x,n,b,PORTB);
  if(b==0)bitClear(x,n);
  if(b==1)bitSet(x,n);
  return;
}

int bit(int n)
{
  int res;
  //for(i=0;i<n;i++)res = res*2;
  res = 1 << n;
  return(res);
}


//------ External Interrupts ---------------


void attachInterrupt(int ir,void(*func)(),int mode)
{
  int pin,ok=S_NOK;

  ok = checkRange(S_OK,"interrupt",ir);
  if(ok == S_OK)
    {
      interruptMode[ir] = mode;
      attached[ir]      = YES;
      interrupt[ir]     = func;

      interrupt[ir]();

      pin                 = inrpt[ir];
      g_attachedPin[pin]  = YES;
      g_interruptType[pin]= mode;

      digitalMode[pin]  = mode;
    }
  else
    {
      errorLog("attachInterruptERROR",ir);
    }

  if(mode==LOW)
    servuinoFunc(S_ATTACH_INTERRUPT_LOW,pin,mode,NULL);
  if(mode==RISING)
    servuinoFunc(S_ATTACH_INTERRUPT_RISING,pin,mode,NULL);
  if(mode==FALLING)
    servuinoFunc(S_ATTACH_INTERRUPT_FALLING,pin,mode,NULL);
  if(mode==CHANGE)
    servuinoFunc(S_ATTACH_INTERRUPT_CHANGE,pin,mode,NULL);
  return;
}

void attachInterruptX(int x,int ir,void(*func)(),int mode)
{
  ino(x);
  attachInterrupt(ir,func,mode);

  return;
}

//---------------------------------------------------
void detachInterrupt(int ir)
{
  int ok=S_NOK,pin;

  
  ok = checkRange(S_OK,"interrupt",ir);
  if(ok == S_OK)
    {
      //interrupt[ir]     = NULL;
      attached[ir]      = NO;
      pin               = inrpt[ir];
      g_attachedPin[pin]= NO;
      digitalMode[pin]  = INPUT;
    }
  
  servuinoFunc(S_DETACH_INTERRUPT,pin,0,NULL);
  return;
}
void detachInterruptX(int x,int ir)
{
  ino(x);
  detachInterrupt(ir);

  return;
}
//------ Interrupts ------------------------
void interrupts()
{
  servuinoFunc(S_UNIMPLEMENTED,0,0,"interrupts()");
}
void noInterrupts()
{
  servuinoFunc(S_UNIMPLEMENTED,0,0,"noInterrupts()");
}
//------ Communication ---------------------
class serial {

 public:
  void begin(int baudRate);
  void end();
  int  available();
  char read();
  int  peek();
  void flush();
  void print(int x);
  void print(int x,int base);
  void print(const char *p);
  void println(int x);
  void println(const char *p);
  void println(string p);
  void println(String p);
  void println();
  void write(char *p);

  // Wrappers
  void beginX(int z,int baudRate);
  void endX(int z);
  int  availableX(int z);
  char readX(int z);
  int  peekX(int z);
  void flushX(int z);
  void printX(int z,int x);
  void printX(int z,int x,int base);
  void printX(int z,const char *p);
  void printlnX(int z,int x);
  void printlnX(int z,const char *p);
  void printlnX(int z,string p);
  void printlnX(int z,String p);
  void printlnX(int z);
  void writeX(int z,char *p);
};

serial Serial,Serial1,Serial2,Serial3;

void serial::begin(int baudRate) 
{
  digitalMode[0] = RX;
  digitalMode[1] = TX;

  servuinoFunc(S_SERIAL_BEGIN,baudRate,0,NULL);
  return;
}
void serial::beginX(int z,int baudRate) 
{
  ino(z);
  begin(baudRate);
  return;
}

void serial::end() 
{
  digitalMode[0] = FREE;
  digitalMode[1] = FREE;

  servuinoFunc(S_SERIAL_END,0,0,NULL);
  return;
}
void serial::endX(int z) 
{
  ino(z);
  end();
  return;
}

int serial::available()  // returns the number of bytes available to read
{
  servuinoFunc(S_UNIMPLEMENTED,0,0,"Serial.available()");
  return(1);
}
int serial::availableX(int z) 
{
  ino(z);
  return(available());
}

char serial::read() // the first byte of incoming serial data available (or -1 if no data is available)
{
  servuinoFunc(S_UNIMPLEMENTED,0,0,"Serial.read()");
  return(-1);
}

int serial::peek() 
{
  servuinoFunc(S_UNIMPLEMENTED,0,0,"Serial.peek()");
  return(-1);
}
int serial::peekX(int z) 
{
  ino(z);
  return(peek());
}

void serial::flush() 
{
  servuinoFunc(S_UNIMPLEMENTED,0,0,"Serial.flush()");
}
void serial::flushX(int z) 
{
  ino(z);
  flush();
}

void serial::print(int x) 
{
  servuinoFunc(S_SERIAL_PRINT_INT,x,0,NULL);
  return;
}
void serial::printX(int z,int x) 
{
  ino(z);
  print(x);
  return;
}

void serial::print(int x,int base) 
{
  servuinoFunc(S_SERIAL_PRINT_INT_BASE,x,base,NULL);
  return;
}
void serial::printX(int z,int x, int base) 
{
  ino(z);
  print(x,base);
  return;
}

void serial::print(const char *p) 
{
  servuinoFunc(S_SERIAL_PRINT_CHAR,0,0,p);
  return;
}
void serial::printX(int z,const char *p) 
{
  ino(z);
  print(p);
  return;
}

void serial::println(int x) 
{
  servuinoFunc(S_SERIAL_PRINTLN_INT,x,0,NULL);
  return;
}
void serial::printlnX(int z,int x) 
{
  ino(z);
  println(x);
  return;
}

void serial::println(const char *p) 
{
  servuinoFunc(S_SERIAL_PRINTLN_CHAR,0,0,p);
  return;
}
void serial::printlnX(int z,const char *p) 
{
  ino(z);
  println(p);
  return;
}

void serial::println(string s) 
{
  const char *p;
  p=s.c_str();
  servuinoFunc(S_SERIAL_PRINTLN_STRING,0,0,p);
  return;
}
void serial::printlnX(int z,string s) 
{
  ino(z);
  println(s);
  return;
}

void serial::println(String s) 
{
  const char *p;
  p= s.getPointer();
  servuinoFunc(S_SERIAL_PRINTLN_SSTRING,0,0,p);
  return;
}
void serial::printlnX(int z, String s) 
{
  ino(z);
  println(s);
  return;
}

void serial::println() 
{
  servuinoFunc(S_SERIAL_PRINTLN_VOID,0,0,NULL);
  return;
}
void serial::printlnX(int z) 
{
  ino(z);
  println();
  return;
}

void serial::write(char *p) 
{
  servuinoFunc(S_SERIAL_WRITE,0,0,p);
  return;
}
void serial::writeX(int z,char *p) 
{
  ino(z);
  println(p);
  return;
}

//==========================================
// String
//==========================================

// conversion (and default) constructor converts char * to String
String::String( const char *s ) 
   : lngth( ( s != 0 ) ? strlen( s ) : 0 )
{
  //cout << "Conversion (and default) constructor: " << s << endl;
   setString( s ); // call utility function
} 

// copy constructor
String::String( const String &copy ) 
   : lngth( copy.lngth )
{
  //cout << "Copy constructor: " << copy.sPtr << endl;
   setString( copy.sPtr ); // call utility function
}

// Destructor
String::~String()
{
  //cout << "Destructor: " << sPtr << endl;
   delete [] sPtr; // release pointer-based string memory
} 

// overloaded = operator; avoids self assignment
const String &String::operator=( const String &right )
{
  //cout << "operator= called" << endl;

   if ( &right != this ) // avoid self assignment
   {         
      delete [] sPtr; // prevents memory leak
      lngth = right.lngth; // new String length
      setString( right.sPtr ); // call utility function
   } // end if
   else
      cout << "Attempted assignment of a String to itself" << endl;

   return *this; 
} 

// concatenate right operand to this object and store in this object
const String &String::operator+=( const String &right )
{
   size_t newLength = lngth + right.lngth; // new length
   char *tempPtr = new char[ newLength + 1 ]; // create memory

   strcpy( tempPtr, sPtr ); // copy sPtr
   strcpy( tempPtr + lngth, right.sPtr ); // copy right.sPtr

   delete [] sPtr; // reclaim old space
   sPtr = tempPtr; // assign new array to sPtr
   lngth = newLength; // assign new length to length
   return *this; // enables cascaded calls
} 

// is this String empty?
bool String::operator!() const
{ 
   return lngth == 0; 
} 

// Is this String equal to right String?
bool String::operator==( const String &right ) const
{ 
   return strcmp( sPtr, right.sPtr ) == 0; 
}

// Is this String less than right String?
bool String::operator<( const String &right ) const
{ 
   return strcmp( sPtr, right.sPtr ) < 0; 
} 

// return reference to character in String as a modifiable lvalue
char &String::operator[]( int subscript )
{
   // test for subscript out of range
   if ( subscript < 0 || subscript >= lngth )
   {
      cerr << "Error: Subscript " << subscript 
         << " out of range" << endl;
      exit( 1 ); // terminate program
   } // end if

   return sPtr[ subscript ]; // non-const return; modifiable lvalue
} 

// return reference to character in String as rvalue
char String::operator[]( int subscript ) const
{
   // test for subscript out of range
   if ( subscript < 0 || subscript >= lngth )
   {
      cerr << "Error: Subscript " << subscript 
           << " out of range" << endl;
      exit( 1 ); // terminate program
   } // end if

   return sPtr[ subscript ]; // returns copy of this element
} 

// return a substring beginning at index and of length subLength
String String::operator()( int index, int subLength ) const
{
   // if index is out of range or substring length < 0, 
   // return an empty String object
   if ( index < 0 || index >= lngth || subLength < 0 )  
      return ""; // converted to a String object automatically

   // determine length of substring
   int len;

   if ( ( subLength == 0 ) || ( index + subLength > lngth ) )
      len = lngth - index;
   else
      len = subLength;

   // allocate temporary array for substring and 
   // terminating null character
   char *tempPtr = new char[ len + 1 ];

   // copy substring into char array and terminate string
   strncpy( tempPtr, &sPtr[ index ], len );
   tempPtr[ len ] = '\0';

   // create temporary String object containing the substring
   String tempString( tempPtr );
   delete [] tempPtr;
   return tempString;
} 

int String::getLength() const 
{ 
   return lngth; 
} 

void String::setString( const char *string2 )
{
   sPtr = new char[ lngth + 1 ];

   if ( string2 != 0 ) 
      strcpy( sPtr, string2 );
   else 
      sPtr[ 0 ] = '\0';
} 

ostream &operator<<( ostream &output, const String &s )
{
   output << s.sPtr;
   return output;
} 

istream &operator>>( istream &input, String &s )
{
   char temp[ 100 ];
   input >> setw( 100 ) >> temp;
   s = temp;
   return input;
} 

String String::operator+(String right )
{
  String temp; 
  size_t newLength = lngth + right.lngth;
  char *tempPtr = new char[ newLength + 1 ]; 
  strcpy( tempPtr, sPtr );
  strcpy( tempPtr + lngth, right.sPtr );
  temp.sPtr = tempPtr;
  temp.lngth = newLength;
  return temp;
}

String String::operator+( const int number )
{
  String temp; 
  stringstream ss;
  String right;
  ss << number;
  right = ss.str();
  size_t newLength = lngth + right.lngth;
  char *tempPtr = new char[ newLength + 1 ];
  strcpy( tempPtr, sPtr );
  strcpy( tempPtr + lngth, right.sPtr );
  temp.sPtr = tempPtr;
  temp.lngth = newLength;
  return temp;
} 

String String::operator+( unsigned long number )
{
  String temp; 
  stringstream ss;
  String right;
  ss << number;
  right = ss.str();
  size_t newLength = lngth + right.lngth;
  char *tempPtr = new char[ newLength + 1 ];
  strcpy( tempPtr, sPtr );
  strcpy( tempPtr + lngth, right.sPtr ); 
  temp.sPtr = tempPtr; 
  temp.lngth = newLength; 
  return temp;
}

String String::operator+( const char one )
{
  String temp; 
  stringstream ss;
  String right;
  ss << one;
  right = ss.str();
  size_t newLength = lngth + right.lngth;
  char *tempPtr = new char[ newLength + 1 ]; 
  strcpy( tempPtr, sPtr );
  strcpy( tempPtr + lngth, right.sPtr );
  temp.sPtr = tempPtr;
  temp.lngth = newLength; 
  return temp; 
} 


const String &String::operator=( string right )
{
  char *p;
  lngth = right.length(); 
  p=(char *)(right.c_str());  
  setString(p);  
  return *this; 
}

const String &String::operator=(const char *right )
{
  const char *p;
  lngth = strlen(right);
  p = right;  
  setString(p);
  return *this; 
} 


String::String( const int x ) 
{
  char p[10];
  sprintf(p,"%d",x);
  lngth = strlen(p);
  setString(p);
} 

String::String( const int x, const int base ) 
{
  char p[10];
  if(base == BIN)sprintf(p,"BIN%d",x);
  if(base == DEC)sprintf(p,"DEC%d",x);
  if(base == HEX)sprintf(p,"HEX%d",x);
  lngth = strlen(p);
  setString(p);
} 

char String::charAt( const int n ) 
{
  char res;
  res = sPtr[n];
  return(res);
} 

int String::compareTo( const String s ) 
{
  int res;
  if(*this > s)res = -1;
  if(*this < s)res = 1;
  if(*this == s)res = 0;
  return(res);
} 

String String::concat( const String s ) 
{
  *this += s;
  return *this;
} 

bool String::endsWith( const String s ) 
{
  bool res;
  int pos;
  string p(sPtr);
  pos = lngth - s.lngth;
  res = p.compare(pos,s.lngth,s.sPtr);
  return(res);
} 

bool String::equals( const String s ) 
{
   return strcmp( sPtr, s.sPtr ) == 0; 
} 

bool String::equalsIgnoreCase( const String s ) 
{
  // Not implemented
   return strcmp( sPtr, s.sPtr ) == 0; 
} 

void String::getBytes( int buf[],int *len ) 
{
  int i;
  char *p;

  p = sPtr;
  for(i=0;i<lngth;i++)
    {
      buf[i] = (int)*p;// Issue 14
      p++;
    }
  *len = lngth;
  return; 
} 

int String::indexOf(char val) 
{
  // Not implemented
  return(-1); 
} 

int String::indexOf(String val) 
{
  // Not implemented
  return(-1); 
} 

int String::indexOf(char val, int from) 
{
  // Not implemented
  return(-1); 
} 

int String::indexOf(String val, int from) 
{
  // Not implemented
  return(-1); 
} 

int String::lastIndexOf(char val) 
{
  // Not implemented
  return(-1); 
} 

int String::lastIndexOf(String val) 
{
  // Not implemented
  return(-1); 
} 

int String::lastIndexOf(char val, int from) 
{
  // Not implemented
  return(-1); 
} 

int String::lastIndexOf(String val, int from) 
{
  // Not implemented
  return(-1); 
} 

int String::length() 
{
  return(lngth); 
} 

String String::replace(String sub1, String sub2) 
{
  // Not implemented
  String res;
  return(res); 
} 

void String::setCharAt( int index,char c ) 
{
  // Not implemented
  return;
} 

bool String::startsWith( String s ) 
{
  bool res;
  // Not implemented
  return(res);
} 

String String::substring(int from) 
{
  String res;
  // Not implemented
  return(res);
} 

String String::substring(int from, int to) 
{
  String res;
  // Not implemented
  return(res);
} 

void String::toCharArray(char buf[], int *len) 
{
  // Not implemented
  return;
} 

void String::toLowerCase() 
{
  // Not implemented
  return;
} 

void String::toUpperCase() 
{
  // Not implemented
  return;
} 

void String::trim() 
{
  // Not implemented
  return;
} 

char* String::getPointer() 
{
  return(sPtr);
} 

//=====================================
// Function Wrappers
//=====================================

void pinModeX(int x,int pin,int mode)
{
  ino(x);
  pinMode(pin,mode);

}
void digitalWriteX(int x,int pin,int value)
{
  ino(x);
  digitalWrite(pin,value);

}
int digitalReadX(int x,int pin)
{
  ino(x);
  return(digitalRead(pin));

}
void analogWriteX(int x,int pin,int value)
{
  ino(x);
  analogWrite(pin,value);

}
int analogReadX(int x,int pin)
{
  ino(x);
  return(analogRead(pin));

}


void delayX(int x,int ms)
{
  ino(x);
  delay(ms);

  return;
}

void delayMicrosecondsX(int x,int us)
{
  ino(x);
  delayMicroseconds(us);

  return;
}
// End of file
