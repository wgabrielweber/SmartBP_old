#include "measureRoutine.h"
#include "MAX30105.h"
#include "globalObjects.h"
#include "payload.h"
#include "commTimeManager.h"

// Perform complete measurement and send data via mqtt
void performMeasurement() {   
    // Define constants
    const int MAX_SAMPLES = 500;  // Maximum number of samples
    unsigned int redValues[MAX_SAMPLES];   // Array to store red values
    unsigned int irValues[MAX_SAMPLES];    // Array to store IR values
    unsigned int redReading;
    unsigned int irReading; 
    unsigned int arraySamples = 0;
    unsigned int startMeasure = 0;
    unsigned int measureTime = 0;

    // Auxiliary variable to calculate the measure time
    startMeasure = millis();

    // Start the measure routine
    while (arraySamples < MAX_SAMPLES) {
    particleSensor.check(); //Check the sensor, read up to 3 samples

        while (particleSensor.available()) //do we have new data?
        {
            // Read sensor FIFO
            redReading = particleSensor.getFIFORed();
            irReading = particleSensor.getFIFOIR();
            // Store the readings in the arrays
            redValues[arraySamples] = redReading;
            irValues[arraySamples] = irReading;

            arraySamples++;

            particleSensor.nextSample(); //We're finished with this sample so move to next sample
        }
    }
    // Calculate the measure time
    measureTime = millis() - startMeasure;

    // Once the loop ends, array is full
    Serial.println("Data collection complete. Performing optimization...");

    // Find the minimum values in the arrays
    uint16_t redMin = UINT16_MAX;
    uint16_t irMin = UINT16_MAX;

    for (uint16_t i = 0; i < arraySamples; i++) {
        redMin = (redValues[i] < redMin) ? redValues[i] : redMin;
        irMin = (irValues[i] < irMin) ? irValues[i] : irMin;
    }

    // Subtract the minimum value from all elements
    for (uint16_t i = 0; i < arraySamples; i++) {
        redValues[i] -= redMin;
        irValues[i] -= irMin;
    }

    //String payload = createJsonPayload(arraySamples, redValues, irValues);
    String payload = createStringPayload("ESP32", getEpochTime(), measureTime, redValues, irValues, MAX_SAMPLES);
    client.publish("prototype_esp/data", payload.c_str());
        
    Serial.println("Data published successfully.");
    //Serial.println(payload);
}