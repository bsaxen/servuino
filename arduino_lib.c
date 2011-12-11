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
  wLog0(temp);
  interruptNow();
}

//------ Digital I/O -----------------------
//=====================================
void pinMode(int pin,int mode)
//=====================================
{
  char temp[80];

  currentPin = pin;
  
  passTime();
  if(mode == INPUT || mode == OUTPUT)
    {

      digitalMode[pin] = mode;

      if(mode==INPUT)
	{
	  wLog1("pinMode IN",pin);
	}

      if(mode==OUTPUT)
	{
	  wLog1("pinMode OUT",pin);
	}

    }
  else
    errorLog("pinMode:Unknown Pin Mode",pin);

  interruptNow();
}
//=====================================
void digitalWrite(int pin,int value)
//=====================================
{
  char temp[80];

  currentPin = pin;

  passTime();
  if(digitalMode[pin] == OUTPUT)
    {
      if(value==HIGH)
	{
	  wLog1("digitalWrite HIGH",pin);
	}
      if(value==LOW)
	{
	  wLog1("digitalWrite LOW",pin);
	}
    }
  else
    {
      errorLog("digitalWrite Wrong Pin Mode ",pin);
    }
  interruptNow();
}
//=====================================
int digitalRead(int pin)
//=====================================
{
  int value=0,x;
  char temp[80];

  currentPin = pin;

  passTime();
  if(digitalMode[pin] == INPUT )
    {
      if(pin >= min_digPin && pin <= max_digPin)
	{
	  value = getDigitalPinValue(pin,currentStep);
	}
      else
	errorLog("digitalRead Pin number out of range",max_digPin);
    }
  else
    errorLog("digitalRead: Wrong pin mode",pin);
  
  wLog2("digitalRead",pin,value);
  interruptNow();
  return(value);
}

//------ Analog I/O ------------------------
//=====================================
void analogReference(char type[])
//=====================================
{
  unimplemented("analogReference()");
  //DEFAULT, INTERNAL, INTERNAL1V1, INTERNAL2V56, or EXTERNAL
}

int analogRead(int pin)  // Values 0 to 1023
{

  int value,x;
  char temp[80];

  currentPin = pin;
  passTime();

  if(pin >= min_anaPin && pin <= max_anaPin)
    {
      value = getAnalogPinValue(pin,currentStep);
      if(value > 1023 || value < 0)
	{
	  sprintf(temp,"%d Analog pin=%d value out of range = %d",currentStep,pin,value);
	  errorLog(temp,0);
	  value = 0;
	}

    }
  else
    errorLog("analogRead Pin number out of range",pin);
  
  wLog2("analogRead",pin,value);
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

  passTime();
  currentPin = pin;

  if(boardType == UNO)
    {
      if(pin==3 || pin==5 || pin==6 || pin==9 || pin==10 || pin==11)
	{
	  
	  if(value > 256 || value < 0)
	    {
	      sprintf(temp,"%d AnalogWrite pin=%d value out of range = %d",currentStep,pin,value);
	      wLog0(temp);
	      value = 0;
	    }
	}
      else
	{
	  errorLog("analogWrite: UNO Pin is not of PWM type",pin);
	}
    }

  if(boardType == MEGA)
    {
      if(pin > 1 && pin < 14)
	{
	  
	  if(value > 256 || value < 0)
	    {
	      sprintf(temp,"%d AnalogWrite pin=%d value out of range = %d",currentStep,pin,value);
	      wLog0(temp);
	      value = 0;
	    }
	}
      else
	{
	  errorLog("analogWrite: MEGA Pin is not of PWM type",pin);
	}
    }

  wLog2("analogWrite",pin,value);
  interruptNow();
  return;
}

//------ Advanced I/O ----------------------
void tone(int pin, unsigned int freq)
{
  currentPin = pin;
  unimplemented("tone()");
}

void tone(int pin, unsigned int freq, unsigned long duration)
{
  currentPin = pin;
  unimplemented("tone()");
}

void noTone(int pin)
{
  currentPin = pin;
  unimplemented("noTone()");
}

void shiftOut(int dataPin, int clockPin, int bitOrder, int value)
{
  //bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST.
  unimplemented("shiftOut()");
}

unsigned long pulseIn(int pin, int value)
{
  currentPin = pin;
  unimplemented("pulseIn()");
}

unsigned long pulseIn(int pin, int value, unsigned long timeout)
{
  currentPin = pin;
  unimplemented("pulseIn()");
}

//------ Time ------------------------------

unsigned long millis()
{
  //unimplemented("millis()");
  return(currentStep*100);
}

unsigned long micros()
{
  //unimplemented("micros()");
  return(currentStep*100000);
}

void delay(int ms)
{
  passTime(); 
  wLog1("delay()",ms);
  interruptNow();
  //msleep(ms);
}

void delayMicroseconds(int us)
{
  passTime();
  wLog1("delayMicroseconds()",us);
  interruptNow();
  //msleep(us);
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
  int pin;

  passTime();
  interruptMode[ir] = mode;

  attached[ir] = YES;
  mLog1("Interrupt number",ir);

  if(ir>=0 && ir <=5)
     interrupt[ir] = func;
  
  pin = inrpt[ir];
  digitalMode[pin] == INTERRUPT;


  if(ir < 0 || ir > 5)
    mLog1("Unsupported interrupt number",ir);

  if(mode==LOW)wLog2("attachInterruptLOW",ir,mode);
  if(mode==RISING)wLog2("attachInterruptRISING",ir,mode);
  if(mode==FALLING)wLog2("attachInterruptFALLING",ir,mode);
  if(mode==CHANGE)wLog2("attachInterruptCHANGE",ir,mode);
  interruptNow();
}

//---------------------------------------------------
void detachInterrupt(int interrupt)
{
  passTime();
  if(interrupt == 0)
    {
      interrupt0 = NULL;
    }
  if(interrupt == 1)
    {
      interrupt1 = NULL;
    }
  
  if(interrupt != 0 && interrupt != 1)
    mLog1("Unsupported interrupt number",interrupt);
    
  wLog1("detachInterrupt",interrupt);
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
  wLog1("Serial:begin",baud);
  digitalMode[0] = RX;
  digitalMode[1] = TX;
  serialMode = ON;
  interruptNow();
}

void serial::end() 
{
  passTime();
  wLog0("Serial:end");
  digitalMode[0] = FREE;
  digitalMode[1] = FREE;
  serialMode = OFF;
  interruptNow();
}

int serial::available()  // returns the number of bytes available to read
{
  unimplemented("Serial.available()");
  return(1);
}

int serial::read() // the first byte of incoming serial data available (or -1 if no data is available)
{
  unimplemented("Serial.read()");
  return(-1);
}

int serial::peek() 
{
  return(-1);
}

void serial::flush() 
{
  showSerial("flush",1);
}

void serial::print(int x) 
{
  passTime();
  wLog1("Serial:print(int)",x);
  interruptNow();
}

void serial::print(int x,int base) 
{
  passTime();
  wLog1("Serial:print(int,int)",x);
  interruptNow();
}

void serial::print(const char *p) 
{
  passTime();
  wLogChar1("Serial:print(char)",p);
  interruptNow();
}

void serial::println(int x) 
{
  passTime();
  wLog1("Serial:println(int)",x);
  interruptNow();
}

void serial::println(const char *p) 
{
  passTime();
  wLogChar1("Serial:println(char)",p);
  interruptNow();
}

void serial::println(String p) 
{
  passTime();
  //wLogChar("Serial:println(char)",p,-1);
  interruptNow();
}

void serial::write(char *p) 
{
  passTime();
  wLogChar1("Serial:write(char)",p);
  interruptNow();
}

