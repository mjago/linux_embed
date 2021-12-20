#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include "mcp9808.h"
#include "temperature.h"

i2c_t * i2c0;
i2c_t * i2c1;
float   temp = 22.0;

/******* Connect *******/

i2c_t * i2c_start(char * dev, int addr)
{
  return temp_init(addr, dev);
  printf("\n\n\n");
}

/******* Main *******/

int tempinit(void)
{
  bool return_val = 1;

  i2c0 = i2c_start(I2C_DEV_NAME, I2C_ADDR_0);
  i2c1 = i2c_start(I2C_DEV_NAME, I2C_ADDR_1);
  if((i2c0 != NULL)            &&
     (i2c1 != NULL)            &&
     (verify_manuf_id(i2c0))    &&
     (verify_dev_id(i2c0))      &&
     ( ! write_critical(i2c0))  &&
     ( ! write_cfg(i2c0))        )
    {
      return_val = 0;
    }
  return return_val;
}

bool temphi(void)
{
   return (temp >= 22.0);
}

bool templow(void)
{
   return (temp <= 20.0);
}

void tempprocess(int id, char * stemp)
{
  int i;
  bool return_val = 1;

  switch(id)
  {
    case 0:
      temp = read_temp(i2c0);
      printf("  temperatures: %0.2f, ", temp);
      break;

    default:
      printf("%0.2f     \r", temp);
      fflush(stdout);
      break;
  }
  sprintf(stemp, "%0.2f", temp);
}

bool tempclose(void)
{
  bool return_val = temp_close(i2c0) | temp_close(i2c1);

  if(return_val != 0)
    {
      fprintf(stderr, "Error reading Temperature!\n");
    }

  return return_val;
}
