#ifndef Logs
#define Logs

#include <Arduino.h>
#include <string.h>
#include <ArduinoJson.h>

class ServerLogs{
    private:
        char payload = "";

    public:
        ServerLogs();
        void add(String msg);
        bool send();
};

#endif