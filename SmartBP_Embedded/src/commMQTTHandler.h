#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include "globalObjects.h"      // Include the global espClient declaration
//#include <WiFiClient.h>
//#include <PubSubClient.h>
//#include "commConfig.h"

void connectToMQTT();
void mqttCallback(char* topic, byte* payload, unsigned int length);

#endif
