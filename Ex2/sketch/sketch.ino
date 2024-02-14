// sketch.ino
// main entry points

#include <Arduino.h>
#include <esp_log.h>


// to blink or not to blink...
bool doBlinking = true;
bool printMac = true;
char macStr[18] = { 0 };
int pushButton = 5, externalLED = 6;

void setup() {
  Serial.begin(115200);
  Serial.println("arduino started");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(externalLED, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
} // setup

void loop() {
  Serial.printf("\nahem, hello I am Andreas (merged bin) world\n");

  #ifdef ESP_IDF_VERSION_MAJOR
    Serial.printf( // IDF version
      "IDF version: %d.%d.%d\n",
      ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR, ESP_IDF_VERSION_PATCH
    );
  #endif
  #ifdef ESP_ARDUINO_VERSION_MAJOR
    Serial.printf(
      "ESP_ARDUINO_VERSION_MAJOR=%d; MINOR=%d; PATCH=%d\n",
      ESP_ARDUINO_VERSION_MAJOR, ESP_ARDUINO_VERSION_MINOR,
      ESP_ARDUINO_VERSION_PATCH
    );
  #endif
  #ifdef ARDUINO_FEATHER_ESP32
    Serial.printf("ARDUINO_FEATHER_ESP32 is defined\n");
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    Serial.printf("ARDUINO_ARCH_ESP32 is defined\n");
  #endif
  #if CONFIG_IDF_TARGET_ESP32
    Serial.printf("CONFIG_IDF_TARGET_ESP32\n");
  #endif
  #if CONFIG_IDF_TARGET_ESP32S3
    Serial.printf("CONFIG_IDF_TARGET_ESP32S3\n");
  #endif
  #ifdef ESP_PLATFORM
    Serial.printf("ESP_PLATFORM is defined\n");
  #endif
  #ifdef ESP32
    Serial.printf("ESP32 is defined\n");
  #endif
  #ifdef IDF_VER
    Serial.printf("IDF_VER=%s\n", IDF_VER);
  #endif
  #ifdef ARDUINO
    Serial.printf("ARDUINO=%d\n", ARDUINO);
  #endif
  #ifdef ARDUINO_BOARD
    Serial.printf("ARDUINO_BOARD=%s\n", ARDUINO_BOARD);
  #endif
  #ifdef ARDUINO_VARIANT
    Serial.printf("ARDUINO_VARIANT=%s\n", ARDUINO_VARIANT);
  #endif
  #ifdef ARDUINO_SERIAL_PORT
    Serial.printf("ARDUINO_SERIAL_PORT=%d\n", ARDUINO_SERIAL_PORT);
  #endif
  
  if (digitalRead(pushButton) == LOW){
    Serial.printf("Button pressed\n");
    doBlinking = !doBlinking;
  }



  if(doBlinking) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(2000);
  }
  else{
    digitalWrite(externalLED, HIGH);
    delay(5000);
    digitalWrite(externalLED, LOW);
    delay(2000);
  }

  Serial.printf("Button pressed: %d\n", digitalRead(pushButton));

  if (printMac) {
    esp_read_mac((uint8_t *)macStr, ESP_MAC_WIFI_STA);
    Serial.printf("MAC address: %s\n", macStr);
    
  }
} // loop
