#ifndef HttpManager_def
#define HttpManager_def

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>


class HttpManager{
    private:
        String payload;
        char* host;
        char* port;
        char* rootUrl;
        String token;
        HTTPClient https;
        BearSSL::WiFiClientSecure client;

    public:
        HttpManager(char* host, char* port, char* rootUrl, String token);
        bool connect(char* endpoint);
        bool send(char* requiresBody);
        void disconect();
        String getPayload();
};

#endif