#ifndef DHTManager_def
#define DHTManager_def

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

class DHTManager{
    private:
        DHT dht;

    public:
        DHTManager(uint8_t pin, uint8_t dhtType = DHT11) : dht(pin, dhtType){
            this->dht.begin();
            delay(1000);
        };
        byte get(byte get = (byte)1);
};

#endif