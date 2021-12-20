#include <stdbool.h>
#include <wiringPi.h>
#include "led.h"

#define LED   3

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


