#ifndef _RELAY_H
#define _RELAY_H

#include <stdbool.h>

#define RLY_0 0
#define RLY_1 2

void relay_init(void);
void relay_exit(void);
void heat_switch(bool state);

#endif
