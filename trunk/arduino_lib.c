//================================================
//  Developed by Benny Saxen, ADCAJO
//================================================



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

/*   String& operator+ (const String& x, const String&  y) */
/* { */
/*   String result = *this;   */
/*   //result += other;      */
/*   return result;   */
/* } */


  //String String::operator+(const String &other);
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
  sprintf(temp,"unimplemented: %s\n",f);
  wLog(temp,-1,-1);
}

//------ Digital I/O -----------------------

void pinMode(int pin,int mode)
{
  char temp[80];
  

  passTime();
  if(mode == INPUT || mode == OUTPUT)
    {
      digitalMode[pin] = mode;

      if(mode==INPUT)
	{
	  strcpy(temp,textPinModeIn[pin]);

	  if(strstr(temp,"void"))
	    wLog("pinMode IN",pin,-1);
	  else
	    wLog(temp,pin,-1);
	}

      if(mode==OUTPUT)
	{
	  strcpy(temp,textPinModeOut[pin]);

	  if(strstr(temp,"void"))
	    wLog("pinMode OUT",pin,-1);
	  else
	    wLog(temp,pin,-1);
	}

    }
  else
    {
      showError("Unknown Pin Mode",mode);
      wLog("pinMode ",pin,-1);
    }
}

void digitalWrite(int pin,int value)
{
  char temp[80];

  passTime();

  if(digitalMode[pin] == OUTPUT)
    {
      c_digitalPin[g_now][pin] = value;

      if(value==HIGH)
	{
	  strcpy(temp,textDigitalWriteHigh[pin]);

	  if(strstr(temp,"void"))
	    wLog("digitalWrite HIGH",pin,-1);
	  else
	    wLog(temp,pin,-1);
	}
      if(value==LOW)
	{
	  strcpy(temp,textDigitalWriteLow[pin]);
	  if(strstr(temp,"void"))
	    wLog("digitalWrite LOW",pin,-1);
	  else
	    wLog(temp,pin,-1);
	}
    }
  else
    {
      showError("Wrong pin mode. Should be OUTPUT",pin);
      wLog("digitalWrite",pin,-1);
    }
}

int digitalRead(int pin)
{
  int value=0;
  char temp[80];

  passTime();
  if(digitalMode[pin] == INPUT)
    {

      value = getDigitalPinValue(pin,timeFromStart);
      c_digitalPin[g_now][pin] = value;
     
      strcpy(temp,textDigitalRead[pin]);
      
      if(strstr(temp,"void"))
	wLog("digitalRead",pin,value);
      else
	wLog(temp,pin,value);
    }
  else
    {
      showError("Wrong pin mode. Should be INPUT",pin);
      wLog("digitalRead",pin,value);
    }
  return(value);
}

//------ Analog I/O ------------------------

void analogReference(char type[])
{
  unimplemented("analogReference()");
  //DEFAULT, INTERNAL, INTERNAL1V1, INTERNAL2V56, or EXTERNAL
}

int analogRead(int pin)  // Values 0 to 1023
{

  int value;
  char temp[80];

  passTime();
  value = getAnalogPinValue(pin,timeFromStart);
  c_analogPin[g_now][pin];

  if(value > 1023 || value < 0)
    {
      sprintf(temp,"%d Analog pin=%d value out of range = %d",timeFromStart,pin,value);
      showError(temp,-1);
      value = 0;
    }
  
  strcpy(temp,textAnalogRead[pin]);
  if(confLogLev > 0)
    {
      if(strstr(temp,"void"))
	wLog("analogRead",pin,value);
      else
	wLog(temp,pin,value);
    }
  return(value); 
}

void analogWrite(int pin,int value) 
// Values 0 to 255   PWM: only pin 3,5,6,9,10,11
{
  char temp[80];

  passTime();

  if(digitalMode[pin] != OUTPUT)
    {
      showError("Pin is not in OUPUT mode: ",pin);
      wLog("analogWrite",pin,value);
      return;
    }

  if(pin==3 || pin==5 || pin==6 || pin==9 || pin==10 || pin==11)
    {

      if(value > 256 || value < 0)
	{
	  sprintf(temp,"%d AnalogWrite pin=%d value out of range = %d",timeFromStart,pin,value);
	  showError(temp,-1);
	  value = 0;
	}
      
      c_digitalPin[g_now][pin] = value;

      strcpy(temp,textAnalogWrite[pin]);
      if(confLogLev > 0)
	{
	  if(strstr(temp,"void"))
	    wLog("analogWrite",pin,value);
	  else
	    wLog(temp,pin,value);
	}
    }
  else
    {
      showError("Pin is not of PWM type",pin);
      wLog("analogWrite",pin,value);
    }
  return;
}

//------ Advanced I/O ----------------------
void tone(int pin, unsigned int freq)
{
  unimplemented("tone()");
}

void tone(int pin, unsigned int freq, unsigned long duration)
{
  unimplemented("tone()");
}

void noTone(int pin)
{
  unimplemented("noTone()");
}

void shiftOut(int dataPin, int clockPin, int bitOrder, int value)
{
  //bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST.
  unimplemented("shiftOut()");
}

unsigned long pulseIn(int pin, int value)
{
  unimplemented("pulseIn()");
}

unsigned long pulseIn(int pin, int value, unsigned long timeout)
{
  unimplemented("pulseIn()");
}

//------ Time ------------------------------

unsigned long millis()
{
  //unimplemented("millis()");
  return(timeFromStart*100);
}

unsigned long micros()
{
  //unimplemented("micros()");
  return(timeFromStart*100000);
}

void delay(int ms)
{
  passTime(); 
  wLog("delay",ms,-1);
  //msleep(ms);
}

void delayMicroseconds(int us)
{
  passTime();
  wLog("delayMicroseconds",us,-1);
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


void attachInterrupt(int interrupt,void(*func)(),int mode)
{
  passTime();
  interruptMode[interrupt] = mode;

  if(interrupt == 0)
    {
      interrupt0 = func;
    }
  if(interrupt == 1)
    {
      interrupt1 = func;
    }

  if(interrupt != 0 && interrupt != 1)
    showError("Unsupported interrupt number",interrupt);

  if(confLogLev > 0)wLog("attachInterrupt",interrupt,-1);

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
    showError("Unsupported interrupt number",interrupt);
    
  if(confLogLev > 0)wLog("detachInterrupt",interrupt,-1);
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
  wLog("Serial:begin",baud,-1);
  digitalMode[0] = RX;
  digitalMode[1] = TX;
  serialMode = ON;
}

void serial::end() 
{
  passTime();
  wLog("Serial:end",-1,-1);
  digitalMode[0] = FREE;
  digitalMode[1] = FREE;
  serialMode = OFF;
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
  //sprintf(stemp,"%d",x);
  //showSerial(stemp,0);
  wLog("Serial:print(int)",x,-1);
}

void serial::print(int x,int base) 
{
  passTime();
  //sprintf(stemp,"%d",x);
  //showSerial(stemp,0);
  wLog("Serial:print(int,int)",x,-1);
}

void serial::print(const char *p) 
{
  passTime();
  //sprintf(stemp,"%s",p);
  //showSerial(stemp,0);
  wLogChar("Serial:print(char)",p,-1);
}

void serial::println(int x) 
{
  passTime();
  //sprintf(stemp,"%d",x);
  //showSerial(stemp,1);
  wLog("Serial:println(int)",x,-1);
}

void serial::println(const char *p) 
{
  passTime();
  //sprintf(stemp,"%s",p);
  //showSerial(stemp,1);
  wLogChar("Serial:println(char)",p,-1);
}

void serial::println(String p) 
{
  passTime();
  //sprintf(stemp,"%s",p);
  //showSerial(stemp,1);
  //wLogChar("Serial:println(char)",p,-1);
}

void serial::write(char *p) 
{
  passTime();
  //sprintf(stemp,"%s",p);
  //showSerial(stemp,1);
  wLogChar("Serial:write(char)",p,-1);
}

