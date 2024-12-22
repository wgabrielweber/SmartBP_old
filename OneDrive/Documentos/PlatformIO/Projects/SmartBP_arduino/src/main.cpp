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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 15000; // 15 seconds for publishing
unsigned long lastRequestedPublishTime = 0;

// Define Global instances
MAX30105 ppgSensor;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

long startTime;
byte interruptPin = 2; //Connect INT pin on breakout board to pin 3

void setup() {
    Serial.begin(115200);   // Serial terminal for Debugging
    Wire.begin(); // Initialize I2C bus

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

    // Sensor Setup
    byte ledBrightness = 0x1F;  //Options: 0=Off to 255=50mA
    byte sampleAverage = 8;     //Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 2;           //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    int sampleRate = 1000;       //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 118;       //Options: 69, 118, 215, 411
    int adcRange = 16384;       //Options: 2048, 4096, 8192, 16384

    ppgSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

    ppgSensor.enableAFULL(); //Enable the almost full interrupt (default is 32 samples)

    ppgSensor.setFIFOAlmostFull(3); //Set almost full int to fire at 29 samples

    ppgSensor.shutDown(); // Shut down the sensor, wake up only when a measure will be made

    // Initialize OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("OLED initialization failed. Please check wiring.");
        while (1);
    }
    display.display();
    delay(2000); // Pause for 2 seconds
    display.clearDisplay();

}

void loop() {
    if (!client.connected()) {
        Serial.println("MQTT not connected. Reconnecting...");
        connectToMQTT(); // Reconnect if the client is disconnected
    }

    // Handle MQTT and ensure it is active
    client.loop();

    display.clearDisplay();
    display.display();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,56);             // Start at top-left corner
    display.println(F("Waiting measure"));
    display.display();
    delay(500);
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,56);             // Start at top-left corner
    display.println(F("Waiting measure."));
    display.display();
    delay(500);
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,56);             // Start at top-left corner
    display.println(F("Waiting measure.."));
    display.display();
    delay(500);
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,56);             // Start at top-left corner
    display.println(F("Waiting measure..."));
    display.display();
    delay(500);
}