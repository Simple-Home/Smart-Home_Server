#include "ServerLogs.h"

ServerLogs::ServerLogs(){

}

void ServerLogs::add(String msg)
{
    this->payload.add(msg);
}

bool ServerLogs::send()
{
    if (this->payload){
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println("Logs-json-Error: " + String(jsonError.c_str()));
        #endif
        }
        return false;
}