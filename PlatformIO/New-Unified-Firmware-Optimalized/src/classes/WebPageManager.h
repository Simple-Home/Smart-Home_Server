#ifndef WebPageManager_def
#define WebPageManager_def

#include <Arduino.h>
#include "classes/EepromManager.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

class WebPageManager
{
private:
    bool configPage = true;
    ESP8266WebServer server{80};
    const byte DNS_PORT = 53;
    DNSServer dnsServer;
    String configApName;
    String configApPassword;
    String pageContent;
    String styleContent;
    String scriptContent;
    String pageHtml;
    EepromManager eeprom_storage;

public:
    WebPageManager(String configApName, String configApPassword, EepromManager eeprom_storage);
    void StartPage();
    void ConfigPage();
    void ResponseHandler();
    String wifiScan();
    void Active();
};

#endif