#ifndef WifiManager_def
#define WifiManager_def

#include "Arduino.h"
#include <ESP8266WiFi.h>

class WifiManager{
    private:
        char* ssid;
        char* password;

    public:
        bool check();
        void connect();
};

#endif