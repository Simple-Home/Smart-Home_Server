#include <Arduino.h>
#include <EEPROM.h">

class Eeprom{
    public:
        Eeprom();
        void write();
        void read();
        void save();
        void erase();
};