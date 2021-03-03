#ifndef EepromManager_def
#define EepromManager_def

#include <Arduino.h>
#include <EEPROM.h>

class EepromManager{
    public:
        EepromManager();
        void write(String data, int startAddr);
        void read(int startAddr, int endAddr);
        void erase(int startAddr, int endAddr);
        bool save();
};

#endif