#include <stdbool.h>
#include <wiringPi.h>
#include "led.h"

#define LED   3

bool green_led = 1;

void discrete_led_init(void)
{
    wiringPiSetup();
    pinMode(LED, OUTPUT);
    digitalWrite(LED, green_led);
}

void discrete_led_toggle(void)
{
    green_led = ! green_led;
    digitalWrite(LED, green_led);
}

void discrete_led_exit(void)
{
    pinMode(LED, INPUT);
}


