#include "commWiFiHandler.h"
#include <WiFi.h>
#include "commTimeManager.h"
#include "defines.h"

// Wi-Fi credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

void setupWiFi() {
    Serial.print("Connecting to Wi-Fi: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA); // Set ESP32 to station mode
    WiFi.begin(ssid, password);

    // Wait for connection
    int max_retries = 20;
    int retry_count = 0;
    while (WiFi.status() != WL_CONNECTED && retry_count < max_retries) {
        delay(500);
        Serial.print(".");
        retry_count++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nWi-Fi connection failed! Restarting...");
        delay(5000);
        ESP.restart();
    }
}

void setupWiFiAndTime() {
    setupWiFi();     // Connect to WiFi
    initializeRTC(); // Synchronize time via NTP
}