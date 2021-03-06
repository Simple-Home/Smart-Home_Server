#ifndef EepromManager_def
#define EepromManager_def

#include <Arduino.h>
#include <EEPROM.h>

class EepromManager{
    public:
        EepromManager();
        void write(char* data, int startAddr = 1, int endAddr = 0);
        String read(int startAddr = 1, int endAddr = 1);
        void erase(int startAddr = 1, int endAddr = 1);
        bool save();
};

#endif