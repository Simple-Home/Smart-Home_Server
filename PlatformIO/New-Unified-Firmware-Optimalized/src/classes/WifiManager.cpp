#include "WifiManager.h"

bool WifiManager::check(int timeout) {
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println(_F("Wifi-Waiting"));
    #endif
    while (0 < timeout)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            #ifdef ENABLE_SERIAL_PRINT
                Serial.println(_F("Wifi-Connected"));
            #endif
            return true;
        }
        delay(1000);
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println(_F("Wifi-Connecting"));
            Serial.println(_F("Wifi-Status: ") + String(WiFi.status()));
        #endif
        timeout--;
    }
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println(_F("Wifi-UnableToConnect!"));
        Serial.println(_F("Wifi-Status: ") + String(WiFi.status()));
    #endif
    return false;
}

void WifiManager::connect(char* ssid, char* password) {
    if (!ssid) {
        ssid = this->ssid;
    }
    if (!password) {
        password = this->password;
    }
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println(_F("SSID:") + String(ssid));
        Serial.println(_F("Password:") + String(password));
    #endif
    WiFi.persistent(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    this->ssid = ssid;
    this->password = password;
}