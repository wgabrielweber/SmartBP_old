#include "commTimeManager.h"
//#include "globalObjects.h"      // Include the global espClient declaration
#include <WiFiClient.h>
//#include <PubSubClient.h>
#include "commConfig.h"
//#include <WiFi.h>
//#include "time.h"

// NTP Server and timezone settings
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;       // Adjust for your timezone (e.g., -18000 for EST)
const int daylightOffset_sec = 0;

void initializeRTC() {
    // Synchronize time via NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // Wait until time is synchronized
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
        Serial.println("Waiting for NTP time sync...");
        delay(1000);
    }
    
    // Format the time into a string
    char timeStr[50];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);  // Example format: YYYY-MM-DD HH:MM:SS

    // Print the formatted date and time
    Serial.print("RTC initialized via NTP at: ");
    Serial.println(timeStr);
}

unsigned long getEpochTime() {
    time_t now;
    time(&now); // Get current time
    return now; // Return epoch time
}
