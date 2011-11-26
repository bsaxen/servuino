==================================================
SERVUINO is a Arduino Simulator Encoder. 
Code: c++
Platform: Linux (Ubuntu)
Version: 0.0.1

Developed by Benny Saxen, ADCAJO
==================================================


--------------------------------------------------
1. Get started
--------------------------------------------------
Quick starter guide:

1. Download simuino zip-file from Google Code, project simuino

2. Unzip the file. A directory named simuino will be created.

3. Go to directory servuino. Compile: g++ -O2 -o servuino servuino.c

4. Run: ./simuino     
   The setup() function will be executed and halt before loop().
   Make sure the size of your terminal window is big enough. 

5. Press "r" to run one loop() sequence. Can be repeated. 
   Press "s" to step one instruction within a loop. Press "r" to complete the loop.

   
6. Quit:  press "q". Works only on loop-level, i e the loop has to be completed.


Test your own sketch in simuino:

1. Copy your sketch (pde-file) to directory/file "simuino/sketch/sketch.pde"

2. Compile "g++ -O2 -o simuino simuino.c -lncurses"

3. Run: ./servuino    


--------------------------------------------------
3. Input
-------------------------------------------------- 

In order to be able to run specific scenarios, controlled by values read from pins,
it is possible to specify what value shall be read per step/instruction and pin.
Only changes is needed to be specified, example:

10   123
14   150
20    45

will generate:

10   123
11   123
12   123
13   123
14   150
15   150
16   150
17   150
18   150
19   150
20    45
21    45
 
and so on...

The scenario file, scenario.txt is located in the subdirectory "scenario".

Below is an example of a scenario file.
-----------------------------------------

#  Simuino scenario file
# start_digital_pins
#   0  1  2  3  4  5  6  7  8  9 10 11 12 13
#-------------------------------------------
0   1  0  1  0  0  0  0  0  0  0  0  0  0  0
100 0  1  0  0  0  0  0  0  0  0  0  0  0  0
200 1  0  1  0  0  0  0  0  0  0  0  0  0  0
300 1  1  0  0  0  0  0  0  0  0  0  0  0  0
# end_digital_pins
# start_analog_pins
#     0   1   2   3   4   5
#--------------------------
0    37   0   0   0   0   0 
100  56   0   0   0   0   0 
200   0   0   0   0   0   0 
300 123   0   0   0   0   0 
# end_analog pins
# start_interrupts
#  0 1
#-----
16 1 0
19 0 0
25 1 0
30 1 1
40 0 1
55 1 1
# end_interrupts

-------------------
End of example


Note - Servuino is uses the tags: start_  end_  for reading correct values from the file.


--------------------------------------------------
5. Supported Language Functions
--------------------------------------------------
Unsupported functions are implemented with a dummy, in order to compile without errors.

Digital I/O
	pinMode()		Yes
	digitalWrite()		Yes
	digitalRead()		Yes
Analog I/O
	analogReference()	No
	analogRead()		Yes
	analogWrite() - PWM	Yes
Advanced I/O
	tone()			No
	noTone()		No
	shiftOut()		No
	shiftIn()		No
	pulseIn()		No
Time
	millis()		Yes
	micros()		Yes
	delay()			Yes
	delayMicroseconds()	Yes
Math
	min()			Yes
	max()			Yes
	abs()			Yes
	constrain()		Yes
	map()			Yes
	pow()			Yes
	sqrt()			Yes
Trigonometry
	sin()			Yes
	cos()			Yes
	tan()			Yes
	Random Numbers		
	       randomSeed()	Yes
	       random()		Yes
Bits and Bytes
	lowByte()		No
	highByte()		No
	bitRead()		No
	bitWrite()		No
	bitSet()		No
	bitClear()		No
	bit()			No
External Interrupts
	attachInterrupt()	Yes
	detachInterrupt()	Yes
Interrupts
	interrupts()		No	
	noInterrupts()		No
Communication
	Serial			
		begin()		Yes
		end()		Yes
		available()	No
		read()		No
		peek()		No
		flush()		Yes
		print()		Yes
		println()	Yes
		write()		Yes
--------------------------------------------------
6. Sketch information
--------------------------------------------------

Customized logging:

Add the following rows in your sketch with suitable log-text according to your specific sketch.

Example:
// SKETCH_NAME           "your name of the sketch"
// PINMODE_OUT        2  "your log text for pin 2"
// PINMODE_IN         4  "your log text for pin 4"
// DIGITALWRITE_LOW  11  "your log text for pin 11 when HIGH"
// DIGITALWRITE_HIGH 11  "your log text for pin 11 when LOW"
// ANALOGREAD         1  "your log text for pin 1"
// DIGITALREAD       10  "your log text for pin 10"
// ANALOGWRITE       14  "your log text for pin 14"

--------------------------------------------------
End of README
--------------------------------------------------
