// PrAndUpThing.ino

// put your code here
#include <Arduino.h>
#include <Wire.h>
#include <esp_log.h>
#include <PrAndUpThing.h>

int firmwareVersion = 1;
unsigned long pressTime;
bool touching = false;

void setup() {
    setupHelperFunctions();
    initWifiConnection();
    setupWifiPorvisioning();
}

void loop() {
    unsigned long now = millis();

    int tval = touchRead(T4);

    if (tval > 100000) {
        if (!touching) {
            pressTime = now;
            touching = true;
        } else {
            // Serial.println(now - pressTime);
            if (now - pressTime > 0 && now - pressTime < 500) {
                ledOn(0);
            } else if (now - pressTime > 500 && now - pressTime < 1000) {
                ledOn(1);
            } else if (now - pressTime > 1000 && now - pressTime < 1500) {
                ledOn(2);
            } else if (now - pressTime > 1500 && now - pressTime < 2000) {
                ledOn(3);
            } else if (now - pressTime > 2000 && now - pressTime < 2500) {
                ledOn(4);
            } else if (now - pressTime > 2500 && now - pressTime < 2750) {
                ledOn(5);
            } else if (now - pressTime > 3000) {
                allLedOff();
                Serial.println("Searching for updates...");
                handleUpdate();
                touching = false;
            }
        }
    
    } else {
        allLedOn();
        dln(startupDBG, "In Loop");
        allLedOff();
        blink(0, 1, 100);
        blink(5, 1, 100);
        touching = false;
        loopWifiProvisioning();
    }
}
