#ifndef Logs
#define Logs

#include "Arduino.h"

class Logs{
    private:
        char* payload = "";

    public:
        Logs();
        void add();
        bool send();
};

#endif