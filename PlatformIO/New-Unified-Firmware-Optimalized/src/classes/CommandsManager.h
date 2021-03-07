#ifndef CommandsManager_def
#define CommandsManager_def

#include <Arduino.h>

class ComandsManager{
    private:
        char* command;

    public:
        ComandsManager(char* command);
        bool execute();
};

#endif