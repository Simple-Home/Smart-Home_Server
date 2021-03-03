#include "Eeprom.h"

Eeprom::Eeprom()
{
    EEPROM.begin(145);
}

Eeprom::write(String data, int startAddr = 1)
{
    for (int i = 0; i < (int)data.length(); ++i)
    {
        EEPROM.write(start + i, data[i]);
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println("Writing EEPROM" + data[i]);
        #endif
        delay(10);
    }
}

Eeprom::read(int startAddr = 1, int endAddr = 1)
{
    String localString;
    for (int i = min; i < max; ++i)
    {
        if (EEPROM.read(i) == 0)
            break;

        localString += char(EEPROM.read(i));
    }
    return localString;
}

Eeprom::erase(int startAddr = 1, int endAddr = 1)
{
    for (int i = 1; i < (98 + plus); ++i)
    {
        EEPROM.write(i, 0);
    }
}

Eeprom::save(int startAddr = 1, int endAddr = 1)
{
    bool status = EEPROM.commit();
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("eeprom-commit " + ((status) ? "OK" : "failed"));
    #endif
    EEPROM.end();
}