#include <Arduino.h>
#include <EEPROM.h>

class EepromManager{
    public:
        EepromManager();
        void write(String data, int startAddr);
        char* read(int startAddr, int endAddr);
        void erase(int startAddr, int endAddr);
        bool save();
};