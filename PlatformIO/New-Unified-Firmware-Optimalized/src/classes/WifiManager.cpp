#include "config.h"
#include "WifiManager.h"

bool WifiManager::check(int timeout) {
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Wifi-Waiting");
    #endif
    while (0 < timeout)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            #ifdef ENABLE_SERIAL_PRINT
                Serial.println("Wifi-Connected");
            #endif
            return true;
        }
        delay(1000);
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println("Wifi-Connecting");
            Serial.println("Wifi-Status: " + String(WiFi.status()));
        #endif
        timeout--;
    }
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Wifi-UnableToConnect!");
        Serial.println("Wifi-Status: " + String(WiFi.status()));
    #endif
    return false;
}

void WifiManager::connect(String localSsid, String localPassword) {
    if (localSsid == "") {
        localSsid = this->ssid;
    }
    if (localPassword == "") {
        localPassword = this->password;
    }
    if (localSsid != "" && localPassword != "") {
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println("SSID:" + localSsid);
            Serial.println("Password:" + localPassword);
        #endif
        WiFi.persistent(true);
        WiFi.mode(WIFI_STA);
        WiFi.begin(localSsid, localPassword);
        this->ssid = localSsid;
        this->password = localPassword;
    }
}