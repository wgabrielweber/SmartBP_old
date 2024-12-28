#include "payload.h"
#include "commTimeManager.h"

String jsonPayload(unsigned int currentIndex, unsigned int* redValues, unsigned int* greenValues, unsigned int* irValues, unsigned int* timestampValues) {
    StaticJsonDocument <1500> jsonDoc; // Dynamically allocated JSON document
    jsonDoc["deviceId"] = "ESP32";
    jsonDoc["collectedDateTime"] = getEpochTime();

    JsonArray channels = jsonDoc.createNestedArray("channels");

    JsonObject redChannel = channels.createNestedObject();
    redChannel["channelId"] = "Red";
    JsonArray redArray = redChannel.createNestedArray("channelValue");
    for (int i = 0; i < currentIndex; i++) {
        redArray.add(redValues[i]);
    }

    JsonObject greenChannel = channels.createNestedObject();
    greenChannel["channelId"] = "Green";
    JsonArray greenArray = greenChannel.createNestedArray("channelValue");
    for (int i = 0; i < currentIndex; i++) {
        greenArray.add(greenValues[i]);
    }

    JsonObject irChannel = channels.createNestedObject();
    irChannel["channelId"] = "IR";
    JsonArray irArray = irChannel.createNestedArray("channelValue");
    for (int i = 0; i < currentIndex; i++) {
        irArray.add(irValues[i]);
    }

    JsonObject timestampChannel = channels.createNestedObject();
    timestampChannel["channelId"] = "Timestamp";
    JsonArray timestampArray = timestampChannel.createNestedArray("channelValue");
    for (int i = 0; i < currentIndex; i++) {
        timestampArray.add(timestampValues[i]);
    }

    String payload;
    serializeJson(jsonDoc, payload);
    return payload;
}
