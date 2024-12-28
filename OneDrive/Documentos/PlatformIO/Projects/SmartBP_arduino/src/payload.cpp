#include "payload.h"
#include "commTimeManager.h"

String createJsonPayload(unsigned int currentIndex, unsigned int* redValues, unsigned int* irValues) {
    StaticJsonDocument <2000> jsonDoc;
    jsonDoc["deviceId"] = "ESP32";
    jsonDoc["collectedDateTime"] = getEpochTime();

    JsonArray channels = jsonDoc.createNestedArray("channels");

    JsonObject redChannel = channels.createNestedObject();
    redChannel["channelId"] = "Red";
    JsonArray redArray = redChannel.createNestedArray("channelValue");
    for (int i = 0; i < currentIndex; i++) {
        redArray.add(redValues[i]);
    }

    JsonObject irChannel = channels.createNestedObject();
    irChannel["channelId"] = "IR";
    JsonArray irArray = irChannel.createNestedArray("channelValue");
    for (int i = 0; i < currentIndex; i++) {
        irArray.add(irValues[i]);
    }

    String payload;
    serializeJson(jsonDoc, payload);
    return payload;
}


// Function to format the data as a string
String createStringPayload(String deviceId, unsigned long timestamp, unsigned int measureTime, unsigned int redArray[], unsigned int irArray[], int arraySize) {
    String payload = deviceId;  // Add deviceId to the start of the payload
    payload += ";";  // Delimiter between fields

    payload += String(timestamp);  // Add timestamp
    payload += ";";  // Delimiter

    payload += String(measureTime);  // Add measure time
    payload += ";";  // Delimiter

    // Convert red array to a string format
    payload += "[";
    for (int i = 0; i < arraySize; i++) {
        payload += String(redArray[i]);
        if (i < arraySize - 1) payload += ", ";  // Add a comma between values
    }
    payload += "]";  // Close the red array

    payload += ";";  // Delimiter

    // Convert ir array to a string format
    payload += "[";
    for (int i = 0; i < arraySize; i++) {
        payload += String(irArray[i]);
        if (i < arraySize - 1) payload += ",";  // Add a comma between values
    }
    payload += "]";  // Close the ir array

    return payload;  // Return the complete payload string
}
