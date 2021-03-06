#include "config.h"
#include "EepromManager.h"

EepromManager::EepromManager()
{
    EEPROM.begin(145);
}

void EepromManager::write(char* data, int startAddr, int endAddr)
{
    if (endAddr == 0) {
        endAddr = (int)strlen(data);
    }
    for (int i = 0; i < endAddr; ++i)
    {
        if (i < (int)strlen(data)) {
            EEPROM.write(startAddr + i, (char)data[i]);
            #ifdef ENABLE_SERIAL_PRINT
                Serial.println("Writing EEPROM: " + String(data[i]));
            #endif
        } else {
            EEPROM.write(startAddr + i, 0);
        }
        delay(10);
    }
}

String EepromManager::read(int startAddr, int endAddr)
{
    if (startAddr >= endAddr) {
        return "";
    }
    String localString = "";
    for (int i = startAddr; i < endAddr; ++i)
    {
        if (EEPROM.read(i) == 0)
            break;

        localString += String((char)EEPROM.read(i));
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