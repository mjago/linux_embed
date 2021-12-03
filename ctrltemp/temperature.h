#ifndef _TEMPERATURE_H
#define _TEMPERATURE_H
#define I2C_DEV_NAME "/dev/i2c-1"
#define I2C_ADDR_0   0x18
#define I2C_ADDR_1   0x19
#include "mcp9808.h"
typedef mcp9808_t    i2c_t;

int tempinit(void);
bool tempprocess(void);
bool tempclose(void);

#endif /* ! _TEMPERATURE_H */

