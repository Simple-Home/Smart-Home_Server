#include <Arduino.h>
#include <EEPROM.h>

class EepromManager{
    public:
        EepromManager();
        void write(String data, int startAddr = 1);
        char* read(int startAddr = 1, int endAddr = 1);
        void erase(int startAddr = 1, int endAddr = 1);
        bool save();
};