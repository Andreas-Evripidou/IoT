
#include <Arduino.h>
#include <WiFi.h>
#include <WifiLocation.h>

#if __has_include("wificonfig.h")
#include "wificonfig.h"
#else
const char* googleApiKey = "YOUR_GOOGLE_API_KEY";
const char* bingMapsKey = "YOUR_BING_MAPS_KEY";
#endif

WifiLocation location (googleApiKey);


void getGeoLocation () {

    location_t loc = location.getGeoFromWiFi ();

    Serial.println ("Location request data");
    Serial.println (location.getSurroundingWiFiJson () + "\n");
    Serial.println ("Location: " + String (loc.lat, 7) + "," + String (loc.lon, 7));
    //Serial.println("Longitude: " + String(loc.lon, 7));
    Serial.println ("Accuracy: " + String (loc.accuracy));
    Serial.println ("Geolocation result: " + location.wlStatusStr (location.getStatus ()));
}
