#include <arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

#ifdef WIFI_CONFIG_PAGE
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#endif

#if defined(ENCODE_WIFI_SETING_TO_CODE) && defined(WIFI_CONFIG_PAGE)
#error "Define only one of ENCODE_WIFI_SETING_TO_CODE/WIFI_CONFIG_PAGE"
#error "ENCODE_WIFI_SETING_TO_CODE - hard Encode wifi ccredentials to Code"
#error "WIFI_CONFIG_PAGE - Enable usage of EEPROM"
#endif

#ifdef DHT_PIN
#include <Adafruit_Sensor.h>
#include <DHT.h>
#endif

#ifdef SWITCH1_PIN
#if defined(MOMENTARY_SWITCH) && defined(ON_OFF_SWITCH)
#error "Define only one type MOMENTARY_SWITCH/ON_OFF_SWITCH"
#endif
#if !defined(MOMENTARY_SWITCH) && !defined(ON_OFF_SWITCH)
#error "Type of switch need to be defined MOMENTARY_SWITCH/ON_OFF_SWITCH"
#endif
#endif

// #define D0 16;
// #define D1 5;
// #define D2 4;
// #define D3 0;
// #define D4 2;
// #define D5 14;
// #define D6 12;
// #define D7 13;
// #define D8 15;
// #define D9 3;
// #define D10 1;
// #define LED_BUILTIN 16