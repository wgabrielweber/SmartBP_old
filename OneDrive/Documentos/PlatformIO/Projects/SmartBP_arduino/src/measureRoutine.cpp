#include "measureRoutine.h"
#include "MAX30105.h"
#include "globalObjects.h"
#include "payload.h"
#include "commTimeManager.h"
#include "displayFunctions.h"

// Perform complete measurement and send data via mqtt
void performMeasurement() {   
    // Oled New Measure
    displayNewMeasure();
    
    // Wake the sensor
    ppgSensor.wakeUp();

    // Define constants
    const int MAX_SAMPLES = 500;           // Maximum number of samples
    const int EXTRA_SAMPLES = 20;          // Extra samples to exclude invalid readings in the sensor initialization 
    unsigned int redValues[MAX_SAMPLES];   // Array to store red values
    unsigned int irValues[MAX_SAMPLES];    // Array to store IR values
    unsigned int redReading;
    unsigned int irReading; 
    unsigned int arraySamples = 0;
    unsigned int startMeasure = 0;
    unsigned int measureTime = 0;

    // Phase 1: Discard initial invalid samples
    int warmUpSamples = 0;
    while (warmUpSamples < EXTRA_SAMPLES) {
        ppgSensor.check(); // Check the sensor, read up to 3 samples

        while (ppgSensor.available()) {
            // Discard initial readings
            redReading = ppgSensor.getFIFORed();
            irReading = ppgSensor.getFIFOIR();
            warmUpSamples++;
            ppgSensor.nextSample(); // Move to the next sample
        }
    }

    // Auxiliary variable to calculate the measure time
    startMeasure = millis();

    // Phase 2: Collect valid samples
    while (arraySamples < MAX_SAMPLES) {
        ppgSensor.check(); // Check the sensor, read up to 3 samples

        while (ppgSensor.available()) {
            // Read and store valid samples
            redValues[arraySamples] = ppgSensor.getFIFORed();
            irValues[arraySamples] = ppgSensor.getFIFOIR();
            arraySamples++;
            ppgSensor.nextSample(); // Move to the next sample
        }
    }

    // Calculate the measure time
    measureTime = millis() - startMeasure;

    // Shut the sensor down
    ppgSensor.shutDown();

    // Once the loop ends, array is full
    Serial.println("Data collection complete. Performing optimization...");

    // Find the minimum values in the arrays
    unsigned int redMin = 262144;
    unsigned int irMin  = 262144;

    for (unsigned int i = 0; i < arraySamples; i++) {
        redMin = (redValues[i] < redMin) ? redValues[i] : redMin;
        irMin = (irValues[i] < irMin) ? irValues[i] : irMin;
    }

    // Subtract the minimum value from all elements
    for (unsigned int i = 0; i < arraySamples; i++) {
        redValues[i] -= redMin;
        irValues[i] -= irMin;
    }

    //String payload = createJsonPayload(arraySamples, redValues, irValues);    // Send payload in json format
    String payload = createStringPayload("ESP32", getEpochTime(), measureTime, redValues, irValues, MAX_SAMPLES);   // Send payload in string format
    client.publish("prototype_esp/data", payload.c_str());
        
    Serial.println("Data published successfully.");

    // Oled Data Sent
    displayDataSent();
}