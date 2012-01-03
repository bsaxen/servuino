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

//====================================
int setRange(int board)
//====================================
{
  int i,j,minx,maxx;
  char temp[120],message[180];

  max_steps = SCEN_MAX;
  max_loops = MAX_LOOPS;

  max_digVal = 1;
  min_digVal = 0;
  
  max_anaVal = 1023;
  min_anaVal = 0;

  max_pwm    = 255;
  min_pwm    = 0;

  if(board == UNO)
    {
      max_digPin = MAX_PIN_DIGITAL_UNO - 1;
      min_digPin = 0;

      max_anaPin = MAX_PIN_ANALOG_UNO - 1;
      min_anaPin = 0;

      max_irPin = MAX_PIN_IR_UNO - 1;
      min_irPin = 0;
    }
  if(board == MEGA)
    {
      max_digPin = MAX_PIN_DIGITAL_MEGA - 1;
      min_digPin = 0;

      max_anaPin = MAX_PIN_ANALOG_MEGA - 1;
      min_anaPin = 0;

      max_irPin = MAX_PIN_IR_MEGA - 1;
      min_irPin = 0;
    }

  inrpt[0] = IR0;
  inrpt[1] = IR1;
  inrpt[2] = IR2;
  inrpt[3] = IR3;
  inrpt[4] = IR4;
  inrpt[5] = IR5;
}
//====================================
int checkRange(int mode,const char check[],int value)
//====================================
{
  int i,j,minx,maxx,res;
  char temp[120],message[180];

  if(mode==HEAL)
    res = value;
  else
    res = S_OK;

  // set ranges
  if(strstr(check,"digval") != NULL)
    {
      strcpy(message,"Digital Value");
      maxx = max_digVal;
      minx = min_digVal;
    }
  else if(strstr(check,"anaval") != NULL)
    {
      strcpy(message,"Analog Value");
      maxx = max_anaVal;
      minx = min_anaVal;
    }
  else if(strstr(check,"pwmval") != NULL)
    {
      strcpy(message,"PWM Value");
      maxx = max_pwm;
      minx = min_pwm;
    }
  else if(strstr(check,"digpin") != NULL)
    {
      strcpy(message,"Digital Pin");
      maxx = max_digPin;
      minx = min_digPin;
    }
  else if(strstr(check,"anapin") != NULL)
    {
      strcpy(message,"Analog Pin");
      maxx = max_anaPin;
      minx = min_anaPin;
    }
  else if(strstr(check,"step") != NULL)
    {
      strcpy(message,"Step out of range");
      maxx = max_steps;
      minx = 1;
    }
  else if(strstr(check,"loop") != NULL)
    {
      strcpy(message,"Loop out of range");
      maxx = max_loops;
      minx = 0;
    }
  else if(strstr(check,"interrupt") != NULL)
    {
      strcpy(message,"Interrupt out of range");
      maxx = max_irPin;
      minx = min_irPin;
    }
  else
    {
      strcpy(message,"Undefined Range Control");
      maxx = 0;
      minx = 0;
    }

  // evaluate
  if(maxx == 0 && minx == 0)
    {
      sprintf(temp,"Unknown:%s",message);
      errorLog(temp,value);
      res = S_NOK;
    }
  else if(value > maxx)
    {
      sprintf(temp,"%s %d >",message,value);
      errorLog(temp,maxx);
      if(mode==HEAL)
	res = maxx;
      else
	res = S_NOK;
    }
  else if(value < minx)
    {
      sprintf(temp,"%s %d <",message,value);
      errorLog(temp,minx);
      if(mode==HEAL)
	res = minx;
      else 
	res = S_NOK;
    }

  return(res);
}


// End of file
