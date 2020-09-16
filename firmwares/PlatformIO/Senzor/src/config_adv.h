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
#ifdef DHT_PIN
    #include <Adafruit_Sensor.h>
    #include <DHT.h>
#endif