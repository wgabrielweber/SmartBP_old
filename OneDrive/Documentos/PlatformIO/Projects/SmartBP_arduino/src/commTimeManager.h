#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <Arduino.h>
#include <time.h>

void initializeRTC();
unsigned long getEpochTime(); // Returns the current time in epoch format
String getDateTime();         // Returns the current time in datetime format

#endif