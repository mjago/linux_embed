#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
//#include <i2c/smbus.h>
#include <sys/ioctl.h>
#include "smbus.h"
#include "mcp9808.h"

/******** Prototypes ********/

static int i2c_read_word_swapped(int file, char cmd);
static int i2c_write_word_swapped(int file, char cmd, int value);
static int set_i2c_slave(mcp9808_t * s);
static bool get_address(int address, mcp9808_t * s);
static void  error_cleanup(mcp9808_t * s);
static bool get_device(const char * filepath, mcp9808_t * s);
static bool i2c_connect(mcp9808_t * s);

/******** Functions ********/

static int i2c_read_word_swapped(int file, char cmd)
{
  int value = i2c_smbus_read_word_data(file, cmd);
   return (value < 0) ? value : (__u16)__builtin_bswap16((__u16)(value));
}

static int i2c_write_word_swapped(int file, char cmd, int value)
{
  int swap = (value < 0) ? value : (__u16)__builtin_bswap16((__u16)(value));
  return i2c_smbus_write_word_data(file, cmd, swap);
}

/* Set i2c slave */

static int set_i2c_slave(mcp9808_t * s)
{
  return ioctl(s -> file, I2C_SLAVE, s -> address);
}

/* get address */

static bool get_address(int address, mcp9808_t * s)
{
  bool return_val = false;

  if(s != NULL)
    {
      s -> address = address;
      return_val = true;
    }

  return return_val;
}

/* error cleanup */

static void  error_cleanup(mcp9808_t * s)
{
  if(s != NULL)
    {
      if(s -> i2c_device != NULL)
        {
          free(s -> i2c_device);
          s -> i2c_device = NULL;
        }
      free(s);
      s = NULL;
    }
  exit(1);
}

/* get device */

static bool get_device(const char * filepath, mcp9808_t * s)
{
  bool return_val = false;
  int f_len = strlen(filepath) + 1;

  if(s != NULL)
    {
      s -> i2c_device = (char *) malloc(f_len * sizeof(char));
      if(s -> i2c_device == NULL)
        {
          error_cleanup(s);
        }
      else
        {
          strcpy(s -> i2c_device, filepath);
          return_val = true;
        }
    }
  return return_val;
}

/* i2c connect */

static bool i2c_connect(mcp9808_t * s)
{
  bool return_val = false;

  if((s -> file = open(s -> i2c_device, O_RDWR)) < 0)
    {
      error_cleanup(s);
    }
  else
    {
      if(set_i2c_slave(s) < 0)
        {
          error_cleanup(s);
        }
      else
        {
          return_val = true;
        }
    }
  return return_val;
}

/******* parse_temp *******/

static float parse_temp(uint16_t itemp)
{
  float ftemp = itemp & 0x0FFF; // three bytes

  ftemp /= 16.0;
  return (itemp & 0x1000) ? (ftemp - 256.0) : ftemp;
}

/* temp init */

mcp9808_t * temp_init(int address, const char * i2c_device_filepath)
{
  mcp9808_t * return_val = NULL;

  mcp9808_t * s = malloc(sizeof(mcp9808_t));
  if(get_address(address, s))
    {
      if(get_device(i2c_device_filepath, s))
        {
          if(i2c_connect(s))
            {
              return_val = s;
            }
        }
    }

  return return_val;
}

/******* temp close *******/

bool temp_close(mcp9808_t * s)
{
  bool return_val = 1;

  if(s != NULL)
    {
      if(close(s -> file) >= 0)
        {
          free(s -> i2c_device);  // free string
          s -> i2c_device = NULL;
          free(s);                // free structure
          s = NULL;
          return_val = 0;
        }
    }
  return return_val;
}

bool verify_manuf_id(mcp9808_t * s)
{
  int val = i2c_read_word_swapped(s -> file, REG_MID);
  return (val & 0xFFFF) == MANUF_ID;
}

bool verify_dev_id(mcp9808_t * s)
{
  int val = i2c_read_word_swapped(s -> file, REG_DID);
  return (val & 0xFFFF) == DEV_ID;
}

bool write_cfg(mcp9808_t * s)
{
  //  fedcba9876543210
  //  0000 0010 0001 1110
  //  0   2    1    E
  return i2c_write_word_swapped(s -> file, REG_CFG, 0x021E);
}

bool write_critical(mcp9808_t * s)
{
  //  fedcba9876543210
  //  000000110000000
  //  0  1   8   0
  return i2c_write_word_swapped(s -> file, REG_CR, 0x0180);
}

bool write_max(mcp9808_t * s)
{
  //  fedcba9876543210
  //
  //
  return i2c_write_word_swapped(s -> file, REG_CR, 0x0180);
}

bool write_min(mcp9808_t * s)
{
  //  fedcba9876543210
  //
  //
  return i2c_write_word_swapped(s -> file, REG_CR, 0x0180);
}

/******* read_temp *******/

float  read_temp(mcp9808_t * s)
{
  uint16_t raw_temp;

  raw_temp = i2c_read_word_swapped(s -> file, REG_TMP);
  return parse_temp(raw_temp);
}

