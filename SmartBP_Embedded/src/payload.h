#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <ArduinoJson.h>

// Function to create a JSON payload
String jsonPayload(unsigned int currentIndex, unsigned * redValues, unsigned int* greenValues, unsigned int* irValues, unsigned int* timestampValues);

#endif // PAYLOAD_H
