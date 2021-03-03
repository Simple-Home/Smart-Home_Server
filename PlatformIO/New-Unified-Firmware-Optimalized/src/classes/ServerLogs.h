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