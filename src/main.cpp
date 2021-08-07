/*
 * Simple example for how to use multiple SinricPro Switch device:
 * - setup 4 switch devices
 * - handle request using multiple callbacks
 * 
 * If you encounter any issues:
 * - check the readme.md at https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md
 * - ensure all dependent libraries are installed
 *   - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#arduinoide
 *   - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#dependencies
 * - open serial monitor and check whats happening
 * - check full user documentation at https://sinricpro.github.io/esp8266-esp32-sdk
 * - visit https://github.com/sinricpro/esp8266-esp32-sdk/issues and check for existing issues or open a new one
 */

// Uncomment the following line to enable serial debug output
//#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
       #define DEBUG_ESP_PORT Serial
       #define NODEBUG_WEBSOCKETS
       #define NDEBUG
#endif 

#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProSwitch.h"

#define WIFI_SSID         "HUAWEI-YBvn"    
#define WIFI_PASS         "U9q5mhDF"
#define APP_KEY           "82a7c941-1f5b-4213-8573-981a700c6674"
#define APP_SECRET        "6a6672fe-9337-49dc-9e48-eb90cdaf7e3a-ede997ee-4994-46cd-991d-aca2acc320d9"

#define SWITCH_ID_1       "610bf5bbfc89cf405088ea02"

#define PIN_UP            14        // pin for roller up
#define PIN_DOWN          12        // pin for roller down
#define LED_PIN           2
#define CLICK_DELAY       500       // delay for transistor click

#define BAUD_RATE         9600      // Change baudrate to your need



// #define WIFI_SSID         "bestconnect.pl 130"    
// #define WIFI_PASS         "pawel130"
// #define APP_KEY           "f8a06490-7d32-4d07-a861-f6816b5110bb"
// #define APP_SECRET        "e69e639e-8b75-4fcd-950c-ab10c25b0647-0aff0982-55e4-44ac-860e-12701d539538"

// #define SWITCH_ID_1       "6107d3e1fc89cf405088cc5b"

// #define PIN_UP            14        // pin for roller up
// #define PIN_DOWN          12        // pin for roller down
// #define LED_PIN           2
// #define CLICK_DELAY       500       // delay for transistor click

// #define BAUD_RATE         9600      // Change baudrate to your need




// My functions declarations
void rollerDown(uint8_t cnt);
void rollerUp(uint8_t cnt);
void setupGPIO();
void blink(uint8_t cnt);


bool onPowerState1(const String &deviceId, bool &state) {
  Serial.printf("Device 1 turned %s\r\n", state?"on":"off");
  
  if(state == 1){
    rollerDown(2);         // put rollers down when state==1
  }
  else{
    rollerUp(2);           // put rollers up when state==0
  }
  return true; // request handled properly
}



// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    digitalWrite(LED_PIN, 0);               //LED on when connecting or reconnecting
    delay(250);
  }

  blink(5);                                // blink when connected
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}



// setup function for SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); digitalWrite(LED_PIN, 1); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); digitalWrite(LED_PIN, 0); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}



// setup function for GPIO
void setupGPIO(){
  pinMode(PIN_UP, OUTPUT);        // roller pin up
  pinMode(PIN_DOWN, OUTPUT);      // roller pin down
  pinMode(LED_PIN, OUTPUT);       // LED pin

  digitalWrite(PIN_UP, 0);        //basic state - 0
  digitalWrite(PIN_DOWN, 0);      //basic state - 0
  digitalWrite(LED_PIN, 0);       //basic state - 0
}


// function for roller up
void rollerUp(uint8_t cnt){
  for (uint8_t i=0; i<cnt; i++){
    digitalWrite(PIN_UP, 1);
    delay(CLICK_DELAY);
    digitalWrite(PIN_UP, 0);
    delay(CLICK_DELAY*2);
  }
  
}

// function for roller down
void rollerDown(uint8_t cnt){
  for (uint8_t i=0; i<cnt; i++){
    digitalWrite(PIN_DOWN, 1);
    delay(CLICK_DELAY);
    digitalWrite(PIN_DOWN, 0);
    delay(CLICK_DELAY*2);
  }
}

void blink(uint8_t cnt){
  for(uint8_t i=0; i < cnt; i++){
    digitalWrite(LED_PIN, 0);
    delay(250);
    digitalWrite(LED_PIN, 1);
    delay(250);
  }
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  setupGPIO();
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();

  // if(WiFi.status() != WL_CONNECTED){
  //   digitalWrite(LED_PIN, 0);                     // led on when not connected
  // }
  // else{
  //   digitalWrite(LED_PIN, 1);                     // led off when not connected
  // }
  // Serial.printf("d");
}