#ifndef PR_AND_UP_THING_H
#define PR_AND_UP_THING_H

#include <Arduino.h>
#include <WiFi.h> //wifi library
#include <WebServer.h> // HTTP server library
#include <HTTPClient.h> // ESP32 library for making HTTP requests
#include <Update.h>     // OTA update library

void setupWifiPorvisioning(); void loopWifiProvisioning();
void handleUpdate();
void initWifiConnection();
void setupHelperFunctions();

// Variables for wifi connection /////////////////////////////////////////////
#define WIFI_SSID "502_Panorama" // change to your SSID
#define WIFI_PSK "Kyz1sAndreas!"  // change to your password (if any)

// Variables for Access Point ////////////////////////////////////////////////
#define AP_SSID "EVRIPIDOU-" // can be given any ssid
#define AP_PSK "password" // can be given any password

// variables for OTA update (server detatils) ////////////////////////////////
#define FIRMWARE_SERVER_IP_ADDR "192.168.4.2" //change to your server IP
#define FIRMWARE_SERVER_PORT    "8000" //change to your server port

// MAC address ///////////////////////////////////////////////////////////////
extern char MAC_ADDRESS[];
void getMAC(char *);

// LED utilities /////////////////////////////////////////////////////////////
void ledOn(int = -1);
void ledOff(int = -1);
void allLedOff();
void allLedOn();
void blink(int = -1, int = 1, int = 300);
void blinkAll(int = 1, int = 300);
void progressBarLed(float progress);
void updateFinishedLed();

// debugging infrastructure; setting different DBGs true triggers prints ////
#define dbg(b, s) if(b) Serial.print(s)
#define dln(b, s) if(b) Serial.println(s)
#define startupDBG      true
// #define loopDBG         true
// #define monitorDBG      true
#define netDBG          true
// #define miscDBG         true
// #define analogDBG       true
// #define otaDBG          true


// globals for a wifi access point and webserver /////////////////////////////
// extern String apSSID;           // SSID of the AP
extern WebServer webServer;     // a simple web server

void startAP();
void printIPs();

// boilerplate: constants & pattern parts of template
extern const char *boiler[];

typedef struct { int position; const char *replacement; } replacement_t;
void getHtml(String& html, const char *[], int, replacement_t [], int);
// getting the length of an array in C can be complex...
// https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
#define ALEN(a) ((int) (sizeof(a) / sizeof(a[0]))) // only in definition scope!
#define GET_HTML(strout, boiler, repls) \
  getHtml(strout, boiler, ALEN(boiler), repls, ALEN(repls));


// set up for provisioning //////////////////////////////////////////////////
void initWebServer();
void hndlNotFound();
void hndlRoot();
void hndlWifi();
void hndlWifichz();
void hndlStatus();
void apListForm(String& f);
String ip2str(IPAddress address);

// set up for updating firmware //////////////////////////////////////////////
extern int firmwareVersion; // used to check for updates
int doCloudGet(HTTPClient *, String);
void handleOTAProgress(size_t done, size_t total);

#endif