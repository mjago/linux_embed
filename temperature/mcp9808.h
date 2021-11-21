#include <stdbool.h>

#define MCP9808_REG_TMP      0x05

/******** Dev Structure ********/

typedef struct MCP9808_T
{
  int    file;       /* file descriptor   */
  int    address;    /* i2c dev addr      */
  char * i2c_device; /* i2c dev file path */
} mcp9808_t;

/******* Functions *******/

mcp9808_t * temp_init(int address, const char * i2c_device_filepath);
bool        temp_close(mcp9808_t * s);
float       read_temp(mcp9808_t * s);
