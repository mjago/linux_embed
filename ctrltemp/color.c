#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include "ws2811.h"
#include "color.h"

//#define ARRAY_SIZE(stuff) (sizeof(stuff) / sizeof(stuff[0]))
//#define TARGET_FREQ       WS2811_TARGET_FREQ
//#define STRIP_TYPE        WS2811_STRIP_GRB
//#define GPIO_PIN          18
//#define DMA               10
//#define LED_COUNT         10

ws2811_led_t dotcolors[] =
{
    0x00000000,  // off
    0x00200000,  // red
    0x00201000,  // orange
    0x00202000,  // yellow
    0x00002000,  // green
    0x00002020,  // lightblue
    0x00000020,  // blue
    0x00100010,  // purple
    0x00200010,  // pink
};

ws2811_led_t * matrix;
static uint8_t running = 1;

void color_render(ws2811_t * led)
{
    ws2811_return_t ret;
    int x;

    for (x = 0; x < 10; x++)
    {
        led -> channel[0].leds[x] = matrix[x];
    }
    if ((ret = ws2811_render(led)) != WS2811_SUCCESS)
    {
      fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
      exit(1);
    }
    ws2811_render(led);
}

void color_clear(void)
{
    int x;

    for (x = 0; x < 10; x++)
      {
        matrix[x] = dotcolors[OFF];
      }
}

void color_shift(void)
{
    static int idx = 0;
    int temp = idx;
    int i;

    for(i = 9; i >= 0; i--)
    {
        if(idx >= 8) { idx = 0; }
        matrix[i] = dotcolors[idx];
        idx++;
    }
    matrix[0] = dotcolors[OFF];
    matrix[4] = dotcolors[OFF];
    matrix[5] = dotcolors[OFF];
    matrix[9] = dotcolors[OFF];
    idx = temp += 1;
    if(idx >= 8) { idx = 0; }
}

void ctrl_c_handler(int signum)
{
    (void) (signum);
    running = 0;
}

bool is_exit(void)
{
    return ! running;
}

void color_setup_hdl(void)
{
    struct sigaction sa =
    {
        .sa_handler = ctrl_c_handler,
    };

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void color_sequence(ws2811_t * led)
{
        color_shift();
        color_render(led);

}

void color_solid(int color, ws2811_t * led)
{
    int i;

    for(i = 0; i < 10; i++)
      {
        matrix[i] = dotcolors[color];
      }
    color_render(led);
}

void color_init(ws2811_t * led)
{
    ws2811_return_t ret;
    static ws2811_t _led =
    {
      .freq = TARGET_FREQ,
      .dmanum = DMA,
      .channel = {
        [0] = {
          .gpionum = GPIO_PIN,
          .count = LED_COUNT,
          .invert = 0,
          .brightness = 255,
          .strip_type = STRIP_TYPE,
        },
        [1] = {
          .gpionum = 0,
          .count = 0,
          .invert = 0,
          .brightness = 0,
        }
      }
    };
    *led = _led;
    matrix = malloc(sizeof(ws2811_led_t) * 10);
    color_setup_hdl();

    if ((ret = ws2811_init(led)) != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        exit(1);
    }

    color_clear();
    color_render(led);
    usleep(1000000);
}
