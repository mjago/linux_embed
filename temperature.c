#include <unistd.h>
#include <stdio.h>
#include "mcp9808.h"
#include "temperature.h"

/******* Connect *******/

i2c_t * i2c_start(char * dev, int addr)
{
  return temp_init(addr, dev);
}

/******* Disconnect *******/

bool i2c_stop(i2c_t * d)
{
  bool return_val = 0;
  if(temp_close(d))
    {
      return_val = 0;
    }
}

/******* print temp *******/

void print_temp(float temp)
{
  printf("temperature: %0.2f\n", temp);
}

/******* Main *******/

int main(int argc, char **argv)
{
  i2c_t * i2c;
  bool return_val = 1;
  int i;

  i2c = i2c_start(I2C_DEV_NAME, I2C_ADDR_0);

  if(i2c != NULL)
    {
    for(i = 0; i < 1000000; i++)
      {
        print_temp(read_temp(i2c));
        usleep(1000000);
      }

    return_val = i2c_stop(i2c);
    }

  return return_val;
}
