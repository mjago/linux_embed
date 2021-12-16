#ifndef _MQTT_CLIENT_H
#define _MQTT_CLIENT_H

void mqtt_init(void);
void mqtt_temperatures(char * stemp1, char * stemp2);
void mqtt_close(void);

#endif /* _MQTT_CLIENT_H */
