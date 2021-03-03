#ifndef Eeprom
#define Eeprom

#include "Arduino.h"
#include "EEPROM.h"

class Eeprom{
    public:
        void Eeprom();
        void write();
        void read();
        bool save();
        void erase();
};

#endif