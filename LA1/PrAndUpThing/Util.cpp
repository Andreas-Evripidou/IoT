#include "PrAndUpThing.h"

// the leds are as follows:
// 5: red
// 6: red
// 9: yellow
// 10: yellow
// 11: green
// 12: green
int ledsList [] = {5, 6, 9, 10, 11, 12};

void setupHelperFunctions() {
    dln(startupDBG, "Setting up helper functions");
    pinMode(LED_BUILTIN, OUTPUT); // set up GPIO pin for built-in LED
    for (int i = 0; i < 6; i++) {
        pinMode(ledsList[i], OUTPUT);
    }
    allLedOff();
}

void getMAC(char *buf) { // the MAC is 6 bytes, so needs careful conversion...
  uint64_t mac = ESP.getEfuseMac(); // ...to string (high 2, low 4):
  char rev[13];
  sprintf(rev, "%04X%08X", (uint16_t) (mac >> 32), (uint32_t) mac);

  // the byte order in the ESP has to be reversed relative to normal Arduino
  for(int i=0, j=11; i<=10; i+=2, j-=2) {
    buf[i] = rev[j - 1];
    buf[i + 1] = rev[j];
  }
  buf[12] = '\0';
}

void ledOn(int led)  { 
    if (led < 0 || led > 5) {
        digitalWrite(LED_BUILTIN, HIGH);
        return;
    };
    digitalWrite(ledsList[led], HIGH);
 }

void ledOff(int led)  { 
    if (led < 0 || led > 5) {
        digitalWrite(LED_BUILTIN, LOW);
        return;
    };
    digitalWrite(ledsList[led], LOW);
 }

void blink(int led, int times, int pause) {
  ledOff(led);
  for(int i=0; i<times; i++) {
    ledOn(led); delay(pause); ledOff(led); delay(pause);
  }
}

void blinkAll(int times, int pause) {
    allLedOff();
    for(int i=0; i<times; i++) {
        allLedOn(); delay(pause); allLedOff(); delay(pause);
    }
}

void updateFinishedLed() {
    allLedOff();
    blink(5, 3, 50);
    blink(0, 3, 50);
    blink(4, 3, 50);
    blink(1, 3, 50);
    blink(3, 3, 50);
    blink(2, 3, 50);
    blink(3, 3, 50);
    blink(1, 3, 50);
    blink(4, 3, 50);
    blink(0, 3, 50);
    blink(5, 3, 50);
}

void allLedOff() {
    for (int i = 0; i < 6; i++) {
        ledOff(i);
    }
}

void allLedOn() {
    for (int i = 0; i < 6; i++) {
        ledOn(i);
    }
}