//====================================
int setRange(int board)
//====================================
{
  int i,j,minx,maxx;
  char temp[120],message[180];

  max_steps = SCEN_MAX;

  max_digVal = 1;
  min_digVal = 0;
  
  max_anaVal = 1023;
  min_anaVal = 0;

  if(board == UNO)
    {
      max_digPin = 13;
      min_digPin = 0;

      max_anaPin = 5;
      min_anaPin = 0;
    }
  if(board == MEGA)
    {
      max_digPin = 53;
      min_digPin = 0;

      max_anaPin = 15;
      min_anaPin = 0;
    }
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
      strcpy(message,"Steps out of range");
      maxx = max_steps;
      minx = 0;
    }
  else
    {
      strcpy(message,"Range Control");
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
	res = 0;
      else 
	res = S_NOK;
    }

  return(res);
}


// End of file
