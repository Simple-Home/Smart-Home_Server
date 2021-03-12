#include "config.h"
#include "OutputManager.h"

OutputManager::OutputManager(byte pin){
    this->pin = pin;
    pinMode(this->pin, OUTPUT);  
}

void OutputManager::on(){
    digitalWrite(this->pin, LOW);
    delay(200);  
}

void OutputManager::off(){
    digitalWrite(this->pin, HIGH);
    delay(200);  
}