#ifndef DHTManager_def
#define DHTManager_def

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "InputManager.h"

class DHTManager: public InputManager{
    private:
        enum DHTType {
            dht11 = DHT11,
            dht22 = DHT22
        };
        DHT dht;

    public:
        DHTManager(byte pin, DHTType dhtType = dht11);
        byte get(byte get = (byte)1);
};

#endif