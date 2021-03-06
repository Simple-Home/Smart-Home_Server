#ifndef WifiManager_def
#define WifiManager_def

#include <Arduino.h>
#include <ESP8266WiFi.h>

class WifiManager{
    private:
        String ssid = "";
        String password = "";

    public:
        bool check(int timeout = 30);
        void connect(String localSsid = "", String localPassword = "");
};

#endif