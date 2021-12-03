#include <stdbool.h>
#include "color.h"
#include "temperature.h"

int main(int argc, char *argv[])
{
    int  usec      = 0;
    bool toggle    = 1;
    int  color     = -1;
    ws2811_t led;
    int colors[6] = {RED, GREEN, BLUE, YELLOW, CYAN, PURPLE};

    color_init(&led);
    tempinit();

    while ( ! is_exit())
    {
        if(toggle)
        {
          color_sequence(&led);
        }
        else
        {
          color_solid(colors[color], &led);
        }
        usleep(100000);
        usec += 1;
        if(usec > 10)
          {
            toggle = ! toggle;
            usec = 0;
            if( ! toggle)
              {
                color++;
                if(color >= 6)
                  {
                    color = 0;
                  }
                tempprocess();

              }
          }
    }

    color_clear();
    color_render(&led);
    ws2811_fini(&led);
    tempclose();
    printf ("\n");
    exit(0);
}
