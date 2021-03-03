#include "EepromManager.h"

EepromManager::EepromManager()
{
    EEPROM.begin(145);
}

void EepromManager::write(char* data, int startAddr = 1)
{
    for (int i = 0; i < (int)data.length(); ++i)
    {
        EEPROM.write(startAddr + i, data[i]);
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println("Writing EEPROM" + data[i]);
        #endif
        delay(10);
    }
}

char* EepromManager::read(int startAddr = 1, int endAddr = 1)
{
    if (startAddr <  endAddr) {
        return false;
    }
    char* localString;
    for (int i = startAddr; i < endAddr; ++i)
    {
        if (EEPROM.read(i) == 0)
            break;

        localString += char(EEPROM.read(i));
    }
    return localString;
}

void EepromManager::erase(int startAddr = 1, int endAddr = 1)
{
    for (int i = startAddr; i < (98 + endAddr); ++i)
    {
        EEPROM.write(i, 0);
    }
}

bool EepromManager::save()
{
    bool status = EEPROM.commit();
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("eeprom-commit " + ((status) ? "OK" : "failed"));
    #endif
    return status;
}