#include <stdbool.h>

#define REG_CFG      0x01
#define REG_HI       0x02
#define REG_LOW      0x03
#define REG_CR       0x04
#define REG_TMP      0x05
#define REG_MID      0x06
#define REG_DID      0x07
#define REG_RES      0x08

#define MANUF_ID     0x54
#define DEV_ID       0x400

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
bool        verify_manuf_id(mcp9808_t * s);
bool        verify_dev_id(mcp9808_t * s);
bool        write_cfg(mcp9808_t * s);
bool        write_critical(mcp9808_t * s);
bool        write_max(mcp9808_t * s);
bool        write_min(mcp9808_t * s);
float       read_temp(mcp9808_t * s);
