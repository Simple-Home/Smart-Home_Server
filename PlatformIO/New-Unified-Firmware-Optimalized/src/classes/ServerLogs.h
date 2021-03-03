#ifndef ServerLogs_def
#define ServerLogs_def

#include <Arduino.h>
#include <ArduinoJson.h>

class ServerLogs{
    private:
       String msg;

    public:
        ServerLogs();
        void add(String msg);
        bool send();
};

#endif