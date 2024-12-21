#include <Arduino.h>
#include <Wire.h>
#include "commWiFiHandler.h"
#include "commMQTTHandler.h"
//#include "commConfig.h"
#include "commTimeManager.h"
#include "payload.h"
#include "globalObjects.h"
#include "MAX30105.h"

unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 15000; // 15 seconds for publishing
unsigned long lastRequestedPublishTime = 0;
volatile bool startMeasurement = false;

// Define Global instances
MAX30105 particleSensor;

//long startTime;
//long samplesTaken = 0; //Counter for calculating the Hz or read rate
//byte interruptPin = 2; //Connect INT pin on breakout board to pin 3

void setup() {
    Serial.begin(115200);   // Serial terminal for Debugging

    // Initialize WiFi and synchronize time
    setupWiFiAndTime();

    // Connect to MQTT and set callback
    client.setCallback(mqttCallback); 
    connectToMQTT();

    client.subscribe("prototype_esp/command");

    // Initialize sensor
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
    {
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }

    //Let's configure the sensor to run fast so we can over-run the buffer and cause an interrupt
    byte ledBrightness = 0x1F; //Options: 0=Off to 255=50mA
    byte sampleAverage = 8; //Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    int sampleRate = 1000; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 118; //Options: 69, 118, 215, 411
    int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

    particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

    //particleSensor.enableAFULL(); //Enable the almost full interrupt (default is 32 samples)

    //particleSensor.setFIFOAlmostFull(3); //Set almost full int to fire at 29 samples

    //startTime = millis();
}

// Perform complete measurement and send data via mqtt
void performMeasurement() {   
    const int MAX_SAMPLES = 15;
    unsigned int currentIndex = 0;
    unsigned int greenValues[MAX_SAMPLES];
    unsigned int irValues[MAX_SAMPLES];
    unsigned int redValues[MAX_SAMPLES];
    unsigned int timestampValues[MAX_SAMPLES];
    unsigned long initialTime = millis();

    while (currentIndex < MAX_SAMPLES) {
        redValues[currentIndex] = particleSensor.getRed();
        greenValues[currentIndex] = particleSensor.getGreen();
        irValues[currentIndex] = particleSensor.getIR();
        timestampValues[currentIndex] = millis() - initialTime;
        currentIndex++;
    }

    String payload = jsonPayload(currentIndex, redValues, greenValues, irValues, timestampValues);
    client.publish("prototype_esp/data", payload.c_str());
    Serial.println("Data published successfully.");
    Serial.println(payload);
}


void loop() {
    if (!client.connected()) {
        connectToMQTT(); // Reconnect if the client is disconnected
    }

    // Handle MQTT and ensure it is active
    client.loop();

    if (startMeasurement) {
        startMeasurement = false; // Reset flag
        Serial.println("Starting measurement...");
        performMeasurement(); // Perform measurement and publish results
    }

    // Publish at regular intervals (non-blocking)
    if ((millis() - lastPublishTime >= publishInterval) &&
        (millis() - lastRequestedPublishTime >= publishInterval)) {
        
        Serial.println("Waiting for new measure");

        // Update last publish time
        lastPublishTime = millis();
    }
}