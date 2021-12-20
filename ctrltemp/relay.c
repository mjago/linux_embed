#include <wiringPi.h>
#include "relay.h"

void relay_init(void)
{
    pinMode(RLY_0, OUTPUT);
    pinMode(RLY_1, OUTPUT);
    digitalWrite(RLY_0, 1);
    digitalWrite(RLY_1, 1);
}

void relay_exit(void)
{
    pinMode(RLY_0, INPUT);
    pinMode(RLY_1, INPUT);
}

void heat_switch(bool state)
{
    digitalWrite(RLY_0, ! state);
    digitalWrite(RLY_1, ! state);
}
