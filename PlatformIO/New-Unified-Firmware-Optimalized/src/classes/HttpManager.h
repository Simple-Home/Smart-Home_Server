#ifndef HttpManager_def
#define HttpManager_def

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>


class HttpManager{
    private:
        char* payload;
        char* host;
        char* port;
        char* url;
        char* token;
        HTTPClient https;
        BearSSL::WiFiClientSecure client;

    public:
        HttpManager(char* host, char* port, char* url, char* token);
        bool connect();
        bool send(char* requiresBody);
        void disconect();
        char* getPayload();
};

#endif