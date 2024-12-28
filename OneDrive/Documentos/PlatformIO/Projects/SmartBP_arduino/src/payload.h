#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <ArduinoJson.h>

// Function to create a JSON payload
String createJsonPayload(unsigned int currentIndex, unsigned int* redValues, unsigned int* irValues);

// Function to create a String payload
String createStringPayload (String deviceId, unsigned long timestamp, unsigned int measureTime, unsigned int redArray[], unsigned int irArray[], int arraySize);

#endif // PAYLOAD_H
