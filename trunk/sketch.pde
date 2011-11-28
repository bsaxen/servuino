//================================================
//  Developed by Benny Saxen
//
//  2011-11-19
//================================================

//  digital_pins
//           step 0  1  2  3  4  5  6  7  8  9 10 11 12 13
//          --- -------------------------------------------
// SCENDIGPIN 0   1  0  1  0  0  0  0  0  0  0  0  0  0  0
// SCENDIGPIN 100 0  1  0  0  0  0  0  0  0  0  0  0  0  0
// SCENDIGPIN 200 1  0  1  0  0  0  0  0  0  0  0  0  0  0
// SCENDIGPIN 300 1  1  0  0  0  0  0  0  0  0  0  0  0  0
//
//  analog_pins
//            step    0   1   2   3   4   5
//            ------------------------------
// SCENANAPIN   0    37   0   0   0   0  11 
// SCENANAPIN 100    56   0   0   0   0  12 
// SCENANAPIN 200     0   0   0   0   0  13 
// SCENANAPIN 300   123   0   0   0   0  14 
//
//  interrupts
//          step 0 1
//         ----------
// SCENINRPT 16  1 0
// SCENINRPT 19  0 0
// SCENINRPT 25  1 0
// SCENINRPT 30  1 1
// SCENINRPT 40  0 1
// SCENINRPT 55  1 1
//

//================================================
// Simuino Support Info
//================================================
// SKETCH_NAME: Heat_Control
// PINMODE_IN:  2   "init start"
// PINMODE_OUT: 8   "init CCW"
// PINMODE_OUT: 9   "init motor direction"
// PINMODE_OUT: 10  "init step motor"
// PINMODE_OUT: 11  "init motor sleeping"

// DIGITALWRITE_LOW:   2  "MS1 Low"
// DIGITALWRITE_HIGH:  2  "MS1 High"
// DIGITALWRITE_LOW:   3  "MS2 Low"
// DIGITALWRITE_HIGH:  3  "MS2 High"

// DIGITALWRITE_LOW:   6  "LED Status off"
// DIGITALWRITE_HIGH:  6  "LED Status on"
// DIGITALWRITE_LOW:   7  "LED CW off"
// DIGITALWRITE_HIGH:  7  "LED CW on"
// DIGITALWRITE_LOW:   8  "LED CCW off"
// DIGITALWRITE_HIGH:  8  "LED CCW on"
// DIGITALWRITE_LOW:   9  "Direction ->"
// DIGITALWRITE_HIGH:  9  "Direction <-"
// DIGITALWRITE_LOW:  10  "Step >"
// DIGITALWRITE_HIGH: 10  "Step <"
// DIGITALWRITE_LOW:  11  "StepperDriver sleep"
// DIGITALWRITE_HIGH: 11  "StepperDriver awake"


// ANALOGREAD: 5  "Outdoor temp"




//------------------------------------------------
//Theory
// Utomhus sensor: Temp_ute(Celcius) = -2.13xResistor(kOhm)  + 8.57  
//
// Sensorn seriekopplad med 10kOhm
//
// 10/14*5 = 3.57 v    max  vid 0 grader Celcius  => 3.57/5*1024 = 731
// 10/26*5 = 1.92 v   min  vid ca -20 grader Celcius  => 1.92/5*1024 = 393
// map(x,393,731,20,0)  from analogRead to minus degrees Celcius

////// ED_v4  Step Mode Chart //////
//                                //
//   MS1 MS2 Resolution           //
//   L   L   Full step (2 phase)  //
//   H   L   Half step            //
//   L   H   Quarter step         //
//   H   H   Eighth step          //
//                                //
////////////////////////////////////

//================================================
int targetShuntPosition  = 0;
int currentShuntPosition = 0; 
int emergencyStop        = 0;
int minusCelcius         = 0;
int aTempValue           = 0;
int stepMode             = 2;
//-------- DIGITAL PIN settings ------------------
// EasyDriver
int DIR       =  9; 
int STEP      = 10;
int SLEEP     = 11;
int MS1       =  2;
int MS2       =  3;
// Control
int cwLED     =  7;
int ccwLED    =  8;
int statusLED =  6; 
//-------- ANALOGUE PIN settings
int tempOutdoor = 5;

//================================================
//  Function Declarations
//================================================
void turn_cw(int delta);
void turn_ccw(int delta);
int  set_shunt_position(int from, int to);
void blinkStatusLed(int n);

//================================================
void setup()
//================================================
{
  Serial.begin(9600); 

  pinMode(DIR,    OUTPUT);   
  pinMode(STEP,   OUTPUT);
  pinMode(SLEEP,  OUTPUT); 
  pinMode(cwLED,  OUTPUT);
  pinMode(ccwLED, OUTPUT);
  pinMode(statusLED, OUTPUT);
  pinMode(MS1,    OUTPUT);   
  pinMode(MS2,    OUTPUT);


  // Full step
  //digitalWrite(MS1, LOW);  
  //digitalWrite(MS2, LOW); 
  // Half step
  digitalWrite(MS1, HIGH);  
  digitalWrite(MS2, LOW); 
  // Quarter step
  //digitalWrite(MS1, LOW);  
  //digitalWrite(MS2, HIGH); 
  // Eighth step
  //digitalWrite(MS1, HIGH);  
  // digitalWrite(MS2, HIGH); 
}
	 


//================================================ 
void loop()
//================================================
{


  stepMode = 2;

  Serial.print("Step Mode:");
  Serial.println(stepMode);
  aTempValue = analogRead(tempOutdoor); // 4 - 16 kOhm
  Serial.print("Sensor:");
  Serial.println(aTempValue);
  minusCelcius        = map(aTempValue,393,731,20,0);
  Serial.print("Celcius(-):");
  Serial.println(minusCelcius);
  targetShuntPosition = map(minusCelcius,0,10,15,30);
  Serial.print("Angle:");
  Serial.println(targetShuntPosition);
  
  Serial.print(currentShuntPosition); 
  Serial.print("--->");
  Serial.println(targetShuntPosition);
  if(targetShuntPosition < 90 || targetShuntPosition > 0)
    {
      currentShuntPosition = set_shunt_position(currentShuntPosition,targetShuntPosition);
    }
  else
    { 
      blinkStatusLed(3);
    }
  delay(3000); 
}


//================================================
void blinkStatusLed(int n)
//================================================
{
  int i;
  for(i=1;i<=n;i++)
    {
      digitalWrite(statusLED, HIGH); 
      delay(500);
      digitalWrite(statusLED, LOW); 
      delay(500);
    }
}
//================================================
int set_shunt_position(int from, int to)
//================================================
{
  int delta=0;

  // This should not happen
  if(  to > 90 ||   to < 0) return(0);
  if(from > 90 || from < 0) return(0);

  if(from == to) 
    {
      blinkStatusLed(1);
      return(to);
    }

  if(from > to)
    {
      delta = from-to;
      turn_ccw(delta);
    }  
  if(from < to)
    {
      delta = to-from;
      turn_cw(delta);
    }
  return(to);
}
//================================================
void turn_cw(int delta)
//================================================
{
  int i,steps = 0;
  steps = map(delta,0,90,0,500);

  Serial.print("Steps:");
  Serial.println(steps);

  digitalWrite(cwLED, HIGH); 
  digitalWrite(DIR, LOW);                 
  digitalWrite(SLEEP, HIGH); // Set the Sleep mode to AWAKE.
  // Step one degree
  for(i=0;i<=steps;i++)
    {
      digitalWrite(STEP, LOW); 
      digitalWrite(STEP, HIGH);
      delayMicroseconds(1400);       
    }  
  digitalWrite(SLEEP, LOW); // Set the Sleep mode to SLEEP.   
  digitalWrite(cwLED, LOW);    
}

//================================================
void turn_ccw(int delta)
//================================================
{
  int i,steps = 0;
  steps = map(delta,0,90,0,500);

  Serial.print("Steps:");
  Serial.println(steps);

  digitalWrite(ccwLED, HIGH);             
  digitalWrite(DIR, HIGH);                 
  digitalWrite(SLEEP, HIGH); // Set the Sleep mode to AWAKE.
  // Step one degree
  for(i=0;i<=steps;i++)
    {
      digitalWrite(STEP, LOW);    
      digitalWrite(STEP, HIGH);    
      delayMicroseconds(1400);        
    }  
  digitalWrite(SLEEP, LOW); // Set the Sleep mode to SLEEP.  
  digitalWrite(ccwLED, LOW);       
}


//================================================
// End of Sketch
//================================================
