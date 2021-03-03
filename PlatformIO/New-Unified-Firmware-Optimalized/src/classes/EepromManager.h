#ifndef EepromManager_def
#define EepromManager_def

#include <Arduino.h>
#include "EEPROM.h"

class EepromManager{
    public:
        EepromManager();
        void write();
        void read();
        bool save();
        void erase();
};

#endif