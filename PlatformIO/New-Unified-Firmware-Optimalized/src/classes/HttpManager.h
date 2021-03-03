#ifndef HttpManager_def
#define HttpManager_def

#include <Arduino.h>
#include <ESP8266HTTPClient.h>

class HttpManager{
    private:
        char* payload;
        char* host;
        char* port;
        char* url;
        char* token;
        HTTPClient https;
        WiFiClientSecure client;

    public:
        HttpManager(char* host, char* port, char* url, char* token);
        void connect();
        bool send(char* requiresBody);
        void disconect();
        char* getPayload();
};

#endif