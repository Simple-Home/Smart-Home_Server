#include "config.h"
#include "EepromManager.h"

EepromManager::EepromManager()
{
    EEPROM.begin(145);
}

void EepromManager::write(char* data, int startAddr)
{
    for (int i = 0; i < int((int)sizeof(data) - 1); ++i)
    {
        EEPROM.write(startAddr + i, data[i]);
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println("Writing EEPROM" + data[i]);
        #endif
        delay(10);
    }
}

char* EepromManager::read(int startAddr, int endAddr)
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

void EepromManager::erase(int startAddr, int endAddr)
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
        Serial.println("eeprom-commit " + String(status ? "OK" : "failed"));
    #endif
    return status;
}