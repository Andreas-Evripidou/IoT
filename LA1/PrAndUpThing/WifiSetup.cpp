// Adapded from Ex10.cpp/.ino
//
// Initial wifi setup; connect to the network
//

#include "PrAndUpThing.h"

void initWifiConnection() {
    Serial.begin(115200); 
    // get on the network
    dln(startupDBG, "Connecting to Wifi");
    dln(startupDBG, "Wifi SSID: " + String(WIFI_SSID));
    dln(startupDBG, "Wifi PSK: " + String(WIFI_PSK));
    WiFi.begin(WIFI_SSID, WIFI_PSK);
    uint16_t connectionTries = 0;
    Serial.print("trying to connect to Wifi...");
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        blink(0, 1, 300);
        if(connectionTries++ % 75 == 0) Serial.println("");
            delay(250);
    }
    delay(500); // let things settle for half a second
    Serial.println("Connected to Wifi successfully");
}