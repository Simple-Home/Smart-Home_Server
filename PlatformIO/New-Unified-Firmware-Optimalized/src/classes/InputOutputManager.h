#ifndef InputOutputManager_def
#define InputOutputManager_def

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

class InputOutputManager{
    private:
        enum InputOutputType {
            led,
            button,
            switchB,
            dht,
            light,
            battery,
            relay
        };
        int pin;
        DHT dht;
        bool state = false;
        enum DHTType {
            DHT11 = (char*)"DHT11",
            DHT22 = (char*)"DHT22"
        };
        InputOutputType inputOutputType;

    public:
        InputOutputManager(int pin, InputOutputType inputOutputType, DHTType dhtType = DHT11);
        void get();
        void set();
};

#endif