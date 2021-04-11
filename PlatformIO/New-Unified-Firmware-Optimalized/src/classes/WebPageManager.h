#ifndef WebPageManager_def
#define WebPageManager_def

#include <Arduino.h>

class WebPageManager{
    private:
        bool configPage = false;
        ESP8266WebServer server(80);
        const byte DNS_PORT = 53;
        DNSServer dnsServer;
        char* configApName;
        char* configApPassword;

    public:
        WebPageManager(char* configApName, char* configApPassword);
        void StartPage();
        void ConfigPage();
};

#endif