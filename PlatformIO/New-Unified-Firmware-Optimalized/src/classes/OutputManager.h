#ifndef OutputManager_def
#define OutputManager_def

#include <Arduino.h>

class OutputManager{
    private:
        byte pin;
        bool state = false;

    public:
        OutputManager(byte pin);
        void on();
        void off();
};

#endif