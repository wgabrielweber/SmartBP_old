#include "commMQTTHandler.h"
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "defines.h"
#include "globalObjects.h"
#include "commTimeManager.h"
#include "measureRoutine.h"

// MQTT Broker credentials and topics
const char* mqtt_server = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;
const char* mqtt_data_topic = MQTT_DATA_TOPIC;

// Define global instances
WiFiClient espClient;     // Definition for the global instance of WiFiClientSecure
PubSubClient client(espClient); // Definition for the global instance of PubSubClient

void connectToMQTT() {
    client.setServer(mqtt_server, mqtt_port);
    while (!client.connected()) {
        Serial.print("Connecting to MQTT broker...");
        if (client.connect("ESP32Client")) {
            Serial.println("connected");
            client.subscribe(mqtt_data_topic); // Subscribe to a topic
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" trying again in 5 seconds");
            delay(5000);
        }
    }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    // Convert payload to a string
    String message;
    String datetime;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    if (String(topic) == "prototype_esp/command" && message == "new_measure") {
        datetime = getDateTime();
        Serial.print("Received new_measure command: ");
        Serial.println(datetime);
        performMeasurement();
    }
}