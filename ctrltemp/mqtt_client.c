/*******************************************************************************
 * Copyright (c) 2012, 2017 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <MQTTClient.h>
#include "mqtt_client.h"

#define ADDRESS     "tcp://192.168.43.197:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC1       "/sensors/temperature_1"
#define TOPIC2       "/sensors/temperature_2"
#define QOS         1
#define TIMEOUT     10000L

char stemp1[8];
char stemp2[8];
int  rc;
MQTTClient client;
MQTTClient_deliveryToken token;

void mqtt_init(void)
{
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = "pi";
    conn_opts.password = "liberty";
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
}

void mqtt_temperatures(char * stemp1, char * stemp2)
{
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = stemp1;
    pubmsg.payloadlen = (int)strlen(stemp1);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC1, &pubmsg, &token);
    pubmsg.payload = stemp2;
    pubmsg.payloadlen = (int)strlen(stemp2);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC2, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
          "on topic %s for client with ClientID: %s\n",
          (int)(TIMEOUT/1000), stemp1, TOPIC1, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n\n", token);
}

void mqtt_close(void)
{
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
//    return rc;
}

