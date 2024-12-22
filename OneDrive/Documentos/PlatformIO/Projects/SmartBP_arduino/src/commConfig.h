#ifndef CONFIG_H
#define CONFIG_H

// Wi-Fi Credentials
#define WIFI_SSID "VIVOFIBRA REPETIDOR";   
#define WIFI_PASSWORD "poker1234567890";   

// MQTT Broker Details
#define MQTT_SERVER "192.168.15.12";
#define MQTT_PORT 1883

// MQTT Topics
#define MQTT_DATA_TOPIC "prototype_esp/data";
#define MQTT_SUB_TOPIC "prototype_esp/command";

#endif // CONFIG_H