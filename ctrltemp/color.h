#ifndef _COLOR_H
#define _COLOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "ws2811.h"

#define ARRAY_SIZE(stuff) (sizeof(stuff) / sizeof(stuff[0]))
#define TARGET_FREQ       WS2811_TARGET_FREQ
#define STRIP_TYPE        WS2811_STRIP_GRB
#define GPIO_PIN          18
#define DMA               10
#define LED_COUNT         10

#define OFF    0
#define RED    1
#define ORANGE 2
#define YELLOW 3
#define GREEN  4
#define CYAN   5
#define BLUE   6
#define PURPLE 7
#define PINK   8

void color_render(ws2811_t * led);
void color_clear(void);
void color_shift(void);
void ctrl_c_handler(int signum);
void color_setup_hdl(void);
void color_sequence(ws2811_t * led);
void color_solid(int color, ws2811_t * led);
void color_init(ws2811_t * led);
bool is_exit(void);

#endif /* ! _COLOR_H */

