#include "config.h"
#include "InputManager.h"

InputManager::InputManager(byte pin) {
    this->pin = pin;
    pinMode(this->pin, INPUT);
}

byte InputManager::get()
{
    return digitalRead(pin);
}
