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

// Math function min and max
#ifndef max
#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif
  
char  stemp[80];

typedef int byte;
typedef bool boolean;

//------ String Class ---------------------
class String {

 public:
  //String(char p);
  String(const char *p);
  void charAt();
  void compareTo();
  void concat();
  void endsWith();
  void equals();
  void equalsIgnoreCase();
  void getBytes();
  void indexOf();
  void lastIndexOf();
  void length();
  String replace(char x, char y);
  String replace(const char *x, const char *y);
  void setCharAt();
  void startsWith();
  void substring();
  void toCharArray();
  void toLowerCase();
  void toUpperCase();
  void trim();

};

String::String(const char *p) 
{
}
void String::charAt() 
{
}
void String::compareTo() 
{
}
void String::concat() 
{
}
void String::endsWith() 
{
}
void String::equals() 
{
}
void String::equalsIgnoreCase() 
{
}
void String::getBytes() 
{
}
void String::indexOf() 
{
}
void String::lastIndexOf() 
{
}
void String::length() 
{
}
String String::replace(char x,char y) 
{
}
String String::replace(const char *x,const char *y) 
{
}
void String::setCharAt() 
{
}
void String::startsWith() 
{
}
void String::substring() 
{
}
void String::toCharArray() 
{
}
void String::toLowerCase() 
{
}
void String::toUpperCase() 
{
}
void String::trim() 
{
}



//=====================================
// Functions
//=====================================
//====================================
void unimplemented(const char *f)
//====================================
{
  char temp[200];
  passTime();
  sprintf(temp,"unimplemented: %s\n",f);
  wLog0(0,temp);
  wLog0(1,temp);
  interruptNow();
}

//------ Digital I/O -----------------------
//=====================================
void pinMode(int pin,int mode)
//=====================================
{
  char temp[120];
  int ok=S_NOK;

  currentPin = pin;
  
  passTime();

  ok = checkRange(S_OK,"digpin",pin);
  if(ok == S_OK)
    {
      if(mode == INPUT || mode == OUTPUT)
	{
	  
	  digitalMode[pin] = mode;

	  if(mode==INPUT)
	    {
	      wLog1(0,"pinMode IN",pin);
	      strcpy(temp,textPinModeIn[pin]);
	      if(!strstr(temp,"void"))
		wLog1(1,temp,pin);
	      else
	      wLog1(1,"pinMode IN",pin);
	    }
	  
	  if(mode==OUTPUT)
	    {
	      wLog1(0,"pinMode OUT",pin);
	      strcpy(temp,textPinModeOut[pin]);
	      if(!strstr(temp,"void"))
		wLog1(1,temp,pin);
	      else
		wLog1(1,"pinMode OUT",pin);
	    }
	  codeLog(F_PINMODE,pin,mode,0,0,NULL);
	}
      else
	errorLog("pinMode:Unknown Pin Mode",pin);
    }
  
  interruptNow();
}
//=====================================
void digitalWrite(int pin,int value)
//=====================================
{
  char temp[120];
  int ok=S_NOK;

  currentPin = pin;


  passTime();

  ok = checkRange(S_OK,"digpin",pin);
  value = checkRange(HEAL,"digval",value);
  c_digitalPin[pin] = value;
  if(ok == S_OK)
    {
      if(digitalMode[pin] == OUTPUT)
	{
	  if(value==HIGH)
	    {
	      wLog1(0,"digitalWrite HIGH",pin);
	      strcpy(temp,textDigitalWriteHigh[pin]);
	      if(!strstr(temp,"void"))
		wLog1(1,temp,pin);
	      else
		wLog1(1,"digitalWrite HIGH",pin);
	    }
	  if(value==LOW)
	    {
	      wLog1(0,"digitalWrite LOW",pin);
	      strcpy(temp,textDigitalWriteLow[pin]);
	      if(!strstr(temp,"void"))
		wLog1(1,temp,pin);
	      else
	      wLog1(1,"digitalWrite LOW",pin);
	    }
	  codeLog(F_DIGITALWRITE,pin,value,0,0,NULL);
	}
      else
	{
	  errorLog("digitalWrite Wrong Pin Mode ",pin);
	}
    }
  interruptNow();
}
//=====================================
int digitalRead(int pin)
//=====================================
{
  int value=0,x,ok=S_NOK;
  char temp[120];

  currentPin = pin;

  passTime();
  ok = checkRange(S_OK,"digpin",pin);
  if(ok == S_OK)
    {
      if(digitalMode[pin] == INPUT )
	{
	    value = getDigitalPinValue(pin,currentStep);  
	    value = checkRange(HEAL,"digval",value);
	}
      else
	errorLog("digitalRead: Wrong pin mode",pin);
    }
  c_digitalPin[pin] = value;
  wLog2(0,"digitalRead",pin,value);
  strcpy(temp,textDigitalRead[pin]);
  if(!strstr(temp,"void"))
    wLog2(1,temp,pin,value);
  else
    wLog2(1,"digitalRead",pin,value);
  codeLog(F_DIGITALREAD,pin,value,0,0,NULL);
  interruptNow();
  return(value);
}

//------ Analog I/O ------------------------
//=====================================
void analogReference(char type[])
//=====================================
{
  unimplemented("analogReference()");
  codeLog(F_ANALOGREFERENCE,0,0,0,0,NULL);
  //DEFAULT, INTERNAL, INTERNAL1V1, INTERNAL2V56, or EXTERNAL
}

int analogRead(int pin)  // Values 0 to 1023
{

  int value,x;
  char temp[80];
  int ok=S_NOK;

  currentPin = pin;
  passTime();
  ok = checkRange(S_OK,"anapin",pin);
  if(ok == S_OK)
    {
      value = getAnalogPinValue(pin,currentStep);
      value = checkRange(HEAL,"anaval",value);
    }
  c_analogPin[pin] = value;
  wLog2(0,"analogRead",pin,value);
  strcpy(temp,textAnalogRead[pin]);
  if(!strstr(temp,"void"))
    wLog2(1,temp,pin,value);
  else
    wLog2(1,"analogRead",pin,value);
  codeLog(F_ANALOGREAD,pin,value,0,0,NULL);
  interruptNow();
  return(value); 
}
//=====================================
void analogWrite(int pin,int value) 
//=====================================
// Values 0 to 255   
// PWM: only pin 3,5,6,9,10,11  UNO
// PWM: only pin 2 - 13 MEGA
{
  char temp[80];
  int ok=S_NOK;


  passTime();
  currentPin = pin;
  c_digitalPin[pin] = value;
  ok = checkRange(S_OK,"digpin",pin);
  if(ok == S_OK)
    {
      value = checkRange(HEAL,"pwmval",value);
      
      if(boardType == UNO)
	{
	  if(pin!=3 && pin!=5 && pin!=6 && pin!=9 && pin!=10 && pin!=11)
	    errorLog("analogWrite: UNO Pin is not of PWM type",pin);
	}
      
      if(boardType == MEGA)
	{
	  if(pin < 2 || pin > 13)
	      errorLog("analogWrite: MEGA Pin is not of PWM type",pin);
	}
      
    }
  wLog2(0,"analogWrite",pin,value);
  strcpy(temp,textAnalogWrite[pin]);
  if(!strstr(temp,"void"))
    wLog2(1,temp,pin,value);
  else
    wLog2(1,"analogWrite",pin,value);
  codeLog(F_ANALOGWRITE,pin,value,0,0,NULL);
  interruptNow();
  return;
}

//------ Advanced I/O ----------------------
void tone(int pin, unsigned int freq)
{
  currentPin = pin;
  codeLog(F_TONE_int_int,pin,freq,0,0,NULL);
  unimplemented("tone()");
}

void tone(int pin, unsigned int freq, unsigned long duration)
{
  currentPin = pin;
  codeLog(F_TONE_int_int_long,pin,freq,duration,0,NULL);
  unimplemented("tone()");
}

void noTone(int pin)
{
  currentPin = pin;
  codeLog(F_NOTONE,pin,0,0,0,NULL);
  unimplemented("noTone()");
}

void shiftOut(int dataPin, int clockPin, int bitOrder, int value)
{
  //bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST.
  codeLog(F_SHIFTOUT,dataPin,clockPin,bitOrder,value,NULL);
  unimplemented("shiftOut()");
}

int shiftIn(int dataPin, int clockPin, int bitOrder)
{
  //bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST.
  codeLog(F_SHIFTIN,dataPin,clockPin,bitOrder,0,NULL);
  unimplemented("shiftIn()");
}

unsigned long pulseIn(int pin, int value)
{
  currentPin = pin;
  codeLog(F_PULSEIN_int_int,pin,value,0,0,NULL);
  unimplemented("pulseIn()");
}

unsigned long pulseIn(int pin, int value, unsigned long timeout)
{
  currentPin = pin;
  codeLog(F_PULSEIN_int_int_long,pin,value,timeout,0,NULL);
  unimplemented("pulseIn()");
}

//------ Time ------------------------------

unsigned long millis()
{
  return(currentStep*100);
}

unsigned long micros()
{
  return(currentStep*100000);
}

void delay(int ms)
{
  passTime(); 
  wLog1(0,"delay()",ms);
  wLog1(1,"delay()",ms);
  codeLog(F_DELAY,ms,0,0,0,NULL);
  interruptNow();
}

void delayMicroseconds(int us)
{
  passTime();
  codeLog(F_DELAYMICROSECONDS,us,0,0,0,NULL);
  wLog1(0,"delayMicroseconds()",us);
  wLog1(1,"delayMicroseconds()",us);
  interruptNow();
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
unsigned char lowByte(unsigned char x)
{
  unimplemented("lowByte()");
}

unsigned char highByte(unsigned char x)
{
  unimplemented("lowByte()");
}

unsigned char bitRead(unsigned char x)
{
  unimplemented("bitRead()");
}

unsigned char bitWrite(unsigned char x)
{
  unimplemented("bitWrite()");
}

unsigned char bitSet(unsigned char x)
{
  unimplemented("bitSet()");
}

unsigned char bitClear(unsigned char x)
{
  unimplemented("bitClear()");
}

unsigned char bit(unsigned char x)
{
  unimplemented("bit()");
}

//------ External Interrupts ---------------


void attachInterrupt(int ir,void(*func)(),int mode)
{
  int pin,ok=S_NOK;

  passTime();

  ok = checkRange(S_OK,"interrupt",ir);
  if(ok == S_OK)
    {
      interruptMode[ir] = mode;
      attached[ir] = YES;
      interrupt[ir] = func;
      pin = inrpt[ir];
      digitalMode[pin] = mode;

      if(mode==LOW)    wLog2(0,"attachInterruptLOW",ir,mode);
      if(mode==RISING) wLog2(0,"attachInterruptRISING",ir,mode);
      if(mode==FALLING)wLog2(0,"attachInterruptFALLING",ir,mode);
      if(mode==CHANGE) wLog2(0,"attachInterruptCHANGE",ir,mode);

      if(mode==LOW)    wLog2(1,"attachInterruptLOW",ir,mode);
      if(mode==RISING) wLog2(1,"attachInterruptRISING",ir,mode);
      if(mode==FALLING)wLog2(1,"attachInterruptFALLING",ir,mode);
      if(mode==CHANGE) wLog2(1,"attachInterruptCHANGE",ir,mode);
      codeLog(F_ATTACHINTERRUPT,pin,ir,mode,0,NULL);
    }
  else
    {
      wLog2(0,"attachInterruptERROR",ir,mode);
      wLog2(1,"attachInterruptERROR",ir,mode);
    }

  interruptNow();
}

//---------------------------------------------------
void detachInterrupt(int ir)
{
  int ok=S_NOK,pin;
  passTime();
  
  ok = checkRange(S_OK,"interrupt",ir);
  if(ok == S_OK)
    {

      interrupt[ir] = NULL;
      pin = inrpt[ir];
      digitalMode[pin] == FREE;
      wLog1(0,"detachInterrupt",ir);
      wLog1(1,"detachInterrupt",ir);
      codeLog(F_DETACHINTERRUPT,pin,ir,0,0,NULL);
    }
  
  interruptNow();
}

//------ Interrupts ------------------------
void interrupts()
{
  unimplemented("interrupts()");
}
void noInterrupts()
{
  unimplemented("noInterrupts()");
}
//------ Communication ---------------------
class serial {

 public:
  void begin(int baudRate);
  void end();
  int  available();
  int  read();
  int  peek();
  void flush();
  void print(int x);
  void print(int x,int base);
  void print(const char *p);
  void println(int x);
  void println(const char *p);
  void println(String p);
  void write(char *p);
};

serial Serial;

void serial::begin(int baudRate) 
{
  passTime();
  baud = baudRate;
  wLog1(0,"Serial:begin",baud);
  wLog1(1,"Serial:begin",baud);
  codeLog(F_SERIAL_BEGIN,baudRate,0,0,0,NULL);
  digitalMode[0] = RX;
  digitalMode[1] = TX;
  serialMode = ON;
  interruptNow();
}

void serial::end() 
{
  passTime();
  wLog0(0,"Serial:end");
  wLog0(1,"Serial:end");
  codeLog(F_SERIAL_END,0,0,0,0,NULL);
  digitalMode[0] = FREE;
  digitalMode[1] = FREE;
  serialMode = OFF;
  interruptNow();
}

int serial::available()  // returns the number of bytes available to read
{
  unimplemented("Serial.available()");
  codeLog(F_SERIAL_AVAILABLE,0,0,0,0,NULL);
  return(1);
}

int serial::read() // the first byte of incoming serial data available (or -1 if no data is available)
{
  unimplemented("Serial.read()");
  codeLog(F_SERIAL_READ,0,0,0,0,NULL);
  return(-1);
}

int serial::peek() 
{
  codeLog(F_SERIAL_PEEK,0,0,0,0,NULL);
  return(-1);
}

void serial::flush() 
{
  codeLog(F_SERIAL_FLUSH,0,0,0,0,NULL);
  showSerial("flush",1);
}

void serial::print(int x) 
{
  passTime();
  wLog1(0,"Serial:print(int)",x);
  wLog1(1,"Serial:print(int)",x);
  fprintf(x_log,"%d SL [%d]\n",currentStep,x);
  codeLog(F_SERIAL_PRINT_int,x,0,0,0,NULL);
  interruptNow();
}

void serial::print(int x,int base) 
{
  passTime();
  wLog1(0,"Serial:print(int,int)",x);
  wLog1(1,"Serial:print(int,int)",x);
  fprintf(x_log,"%d SL [%d]\n",currentStep,x);
  codeLog(F_SERIAL_PRINT_int_int,x,base,0,0,NULL);
  interruptNow();
}

void serial::print(const char *p) 
{
  passTime();
  codeLog(F_SERIAL_PRINT_char,0,0,0,0,p);
  wLogChar1(0,"Serial:print(char)",p);
  wLogChar1(1,"Serial:print(char)",p);
  fprintf(x_log,"%d SL [%s]\n",currentStep,p);
  interruptNow();
}

void serial::println(int x) 
{
  passTime();
  codeLog(F_SERIAL_PRINTLN_int,x,0,0,0,NULL);
  wLog1(0,"Serial:println(int)",x);
  wLog1(1,"Serial:println(int)",x);
  fprintf(x_log,"%d NL [%d]\n",currentStep,x);
  interruptNow();
}

void serial::println(const char *p) 
{
  passTime();
  codeLog(F_SERIAL_PRINTLN_char,0,0,0,0,p);
  wLogChar1(0,"Serial:println(char)",p);
  wLogChar1(1,"Serial:println(char)",p);
  fprintf(x_log,"%d NL [%s]\n",currentStep,p);
  interruptNow();
}

void serial::println(String p) 
{
  passTime();
  codeLog(F_SERIAL_PRINTLN_String,0,0,0,0,NULL);
  //wLogChar(0,"Serial:println(char)",p,-1);
  //wLogChar(1,"Serial:println(char)",p,-1);
  //fprintf(x_log,"%d NL [%s]\n",currentStep,p);
  interruptNow();
}

void serial::write(char *p) 
{
  passTime();
  codeLog(F_SERIAL_WRITE,0,0,0,0,p);
  wLogChar1(0,"Serial:write(char)",p);
  wLogChar1(1,"Serial:write(char)",p);
  fprintf(x_log,"%d SL [%s]\n",currentStep,p);
  interruptNow();
}

