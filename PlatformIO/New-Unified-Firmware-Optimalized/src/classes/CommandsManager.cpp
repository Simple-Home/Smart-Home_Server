#include "config.h"
#include "ComandsManager.h"

ComandsManager::ComandsManager(char *command)
{
    this->command = command;
}

ComandsManager::execute()
{
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Command-" + String(this->command));
    #endif
    switch (this->command)
    {
    case "reset":
        /* code */
        break;
    case "update":
        /* code */
        break;
    case "config":
        /* code */
        break;
    case "factory":
        /* code */
        break;
    default:
        break;
    }
}