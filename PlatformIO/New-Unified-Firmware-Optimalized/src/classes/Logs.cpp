#include "Logs.h"

Logs::Logs(){

}

void Logs::add(char* msg)
{
    Logs::payload += msg;
}

bool Logs::send()
{
    if (Logs::payload != ""){
        DynamicJsonDocument jsonContent(259);
        jsonError = deserializeJson(jsonContent, "{\"logs\":[" + Logs::payload + "]}");

        if (jsonError.code() == DeserializationError::Ok){
            return /*Send To Server*/;
        }
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println("Logs-json-Error: " + String(jsonError.c_str()));
        #endif
        }
        return false;
}