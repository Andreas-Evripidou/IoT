// WifiLocation.h
// Code taken and adapteded from https://github.com/gmag11/WifiLocation/blob/master/src/WifiLocation.h

#ifndef _WIFILOCATION_h
#define _WIFILOCATION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define DEBUG_WIFI_LOCATION 0
//#define USE_CORE_PRE_2_5_0 // Uncomment this if you are using Arduino Core < 2.5.0

#if defined ESP8266
#include <ESP8266WiFi.h>
#elif defined ESP32
#include <WiFi.h>
#include <WiFiClientSecure.h>
#else
#error Wrong platform
#endif

#define MAX_CONNECTION_TIMEOUT 5000
#define MAX_WIFI_SCAN 127

typedef enum {
    WL_OK = 0,
    WL_UNKNOWN = -1,
    WL_API_CONNECT_ERROR = -2,
    WL_TIME_NOT_SET = -3
} wifiloc_status_t;

typedef struct {
    float lat = 0;
    float lon = 0;
    int accuracy = 40000;
} location_t;

class WifiLocation {
public:
    WifiLocation(String googleKey = "");
    location_t getGeoFromWiFi();
    static String getSurroundingWiFiJson ();
    static String wlStatusStr (int status);
    wifiloc_status_t getStatus () {
        return status;
    }

protected:
    String _googleApiKey;
    WiFiClientSecure  _client;
    wifiloc_status_t status = WL_UNKNOWN;

    static String MACtoString(uint8_t* macAddress);
};

#endif