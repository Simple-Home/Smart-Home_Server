#ifndef InputManager_def
#define InputManager_def

#include <Arduino.h>

class InputManager{
    private:
        byte pin;
        bool state = false;

    public:
        InputManager(byte pin);
        byte get();

};
#endif