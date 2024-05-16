
#include <Arduino.h>
#include <wifiLocation/WifiLocation.h>

#if __has_include("config.h")
#include "config.h"
#else
// const char* googleApiKey = "YOUR_GOOGLE_API_KEY";
#define FIRMWARE_SERVER_IP_ADDR "YOUR_SERVER_IP";
#define FIRMWARE_SERVER_PORT    "YOUR_SERVER_PORT";
#endif

WifiLocation location (googleApiKey);
location_t loc;

int getGeoLocation () {

    Serial.println ("Location request data");
    loc = location.getGeoFromWiFi ();

    String wifiJson = location.getSurroundingWiFiJson ();
    if (wifiJson == "[]") {
        return WL_UNKNOWN;
    } else {
        Serial.println (wifiJson + "\n");
    }
    
    Serial.println ("Location: " + String (loc.lat, 7) + "," + String (loc.lon, 7));
    //Serial.println("Longitude: " + String(loc.lon, 7));
    Serial.println ("Accuracy: " + String (loc.accuracy));
    Serial.println ("Geolocation result: " + location.wlStatusStr (location.getStatus ()));

    return WL_OK;

}
