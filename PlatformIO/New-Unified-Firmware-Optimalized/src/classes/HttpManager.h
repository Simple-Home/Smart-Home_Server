#ifndef HttpManager_def
#define HttpManager_def

#include <Arduino.h>
#include <ESP8266HTTPClient.h>

class HttpManager{
    private:
        char* response;
        char* host;
        char* port;
        char* url;
        char* token
        HTTPClient https;
        WiFiClientSecure client;

    public:
        HttpManager();
        bool connect();
        bool send();
        bool disconect();
};

#endif