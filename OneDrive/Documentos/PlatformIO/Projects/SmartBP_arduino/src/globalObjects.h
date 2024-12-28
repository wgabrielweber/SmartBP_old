#ifndef GLOBALOBJECTS_H
#define GLOBALOBJECTS_H

#include <WiFiClient.h>
#include <PubSubClient.h>
#include "MAX30105.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern WiFiClient espClient; // Declare the global instance
extern PubSubClient client;
extern MAX30105 ppgSensor;
extern Adafruit_SSD1306 display;
extern unsigned long lastRequestedPublishTime;
extern const char* mqtt_data_topic;

#endif