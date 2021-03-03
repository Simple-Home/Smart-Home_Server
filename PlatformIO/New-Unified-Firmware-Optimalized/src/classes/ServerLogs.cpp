#include "ServerLogs.h"

ServerLogs::ServerLogs(){
    this->payload = "";
}

void ServerLogs::add(char msg[])
{
    this->payload += msg;
}

bool ServerLogs::send()
{
    if (this->payload != ""){
        DynamicJsonDocument jsonContent(259);
        DeserializationError jsonError = deserializeJson(jsonContent, "{\"logs\":[" + this->payload + "]}");

        if (jsonError.code() == DeserializationError::Ok){
            return /*Send To Server*/;
        }
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println("Logs-json-Error: " + String(jsonError.c_str()));
        #endif
        }
        return false;
}