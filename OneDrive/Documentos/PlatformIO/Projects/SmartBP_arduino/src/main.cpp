#include <Arduino.h>
#include <Wire.h>
#include "globalObjects.h"
//#include "commConfig.h"
#include "commWiFiHandler.h"
#include "commMQTTHandler.h"
#include "commTimeManager.h"
#include "payload.h"
#include "measureRoutine.h"
#include "MAX30105.h"

unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 15000; // 15 seconds for publishing
unsigned long lastRequestedPublishTime = 0;

// Define Global instances
MAX30105 ppgSensor;

long startTime;
byte interruptPin = 2; //Connect INT pin on breakout board to pin 3

void setup() {
    Serial.begin(115200);   // Serial terminal for Debugging

    // Initialize WiFi and synchronize time
    setupWiFiAndTime();

    // Connect to MQTT and set callback
    client.setCallback(mqttCallback); 
    connectToMQTT();

    client.subscribe("prototype_esp/command");

    // Initialize sensor
    if (!ppgSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
    {
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }

    //Let's configure the sensor to run fast so we can over-run the buffer and cause an interrupt
    byte ledBrightness = 0x1F;  //Options: 0=Off to 255=50mA
    byte sampleAverage = 4;     //Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 2;           //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    int sampleRate = 800;       //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 118;       //Options: 69, 118, 215, 411
    int adcRange = 16384;       //Options: 2048, 4096, 8192, 16384

    ppgSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

    ppgSensor.enableAFULL(); //Enable the almost full interrupt (default is 32 samples)

    ppgSensor.setFIFOAlmostFull(3); //Set almost full int to fire at 29 samples

    ppgSensor.shutDown(); // Shut down the sensor, wake up only when a measure will be made
}

void loop() {
    if (!client.connected()) {
        Serial.println("MQTT not connected. Reconnecting...");
        connectToMQTT(); // Reconnect if the client is disconnected
    }

    // Handle MQTT and ensure it is active
    client.loop();

}