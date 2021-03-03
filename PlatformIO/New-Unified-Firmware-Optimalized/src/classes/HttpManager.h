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
        HttpManager(char* host, char* port, char* url, char* token);
        bool connect();
        bool send(char* requiresBody);
        bool disconect();
        char* getPayload();
};