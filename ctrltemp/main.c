#include <stdbool.h>
#include <wiringPi.h>
#include "color.h"
#include "temperature.h"
#include "mqtt_client.h"

#define LED   3
#define RLY_0 0
#define RLY_1 2

bool green_led = 1;


void discrete_led_init(void)
{
    wiringPiSetup();
    pinMode(LED, OUTPUT);
    pinMode(RLY_0, OUTPUT);
    pinMode(RLY_1, OUTPUT);
    digitalWrite(LED, green_led);
    digitalWrite(RLY_0, 1);
    digitalWrite(RLY_1, 1);
}

void discrete_led_toggle(void)
{
    green_led = ! green_led;
    digitalWrite(LED, green_led);
}

void discrete_led_exit(void)
{
    pinMode(LED, INPUT);
    pinMode(RLY_0, INPUT);
    pinMode(RLY_1, INPUT);
}

int main(int argc, char *argv[])
{
    int  usec      = 0;
    bool toggle    = 1;
    bool heat_on   = 0;
    char stemp0[16];
    char stemp1[16];
    ws2811_t led;

    color_init(&led);
    tempinit();
    discrete_led_init();
    mqtt_init();

    while ( ! is_exit())
    {
        if(toggle)
        {
          color_sequence(&led);
        }
        else
        {
          color_solid(&led);
        }
        usleep(100000);
        usec += 1;
        if(usec > 10)
          {
            toggle = ! toggle;
            usec = 0;
            if( ! toggle)
              {
                discrete_led_toggle();
                tempprocess(0, stemp0);
                tempprocess(1, stemp1);
                if(heat_on)
                {
                  if(temphi())
                  {
                    heat_on = 0;
                    digitalWrite(RLY_0, 1);
                    digitalWrite(RLY_1, 1);
                  }
                }
                else
                {
                  if(templow())
                  {
                    heat_on = 1;
                    digitalWrite(RLY_0, 0);
                    digitalWrite(RLY_1, 0);
                  }
                }
                mqtt_temperatures(stemp0, stemp1);
              }
            else
              {
                discrete_led_toggle();
              }
          }
    }

    color_clear();
    color_render(&led);
    ws2811_fini(&led);
    tempclose();
    discrete_led_exit();
    mqtt_close();
    printf ("\n");
    exit(0);
}
