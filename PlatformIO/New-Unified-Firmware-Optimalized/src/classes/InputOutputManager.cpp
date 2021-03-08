#include "config.h"
#include "InputOutputManager.h"

InputOutputManager::InputOutputManager(int pin, InputOutputType inputOutputType, DHTType dhtType){
    this->inputOutputType = inputOutputType;
    this->pin = pin;
    switch (inputOutputType) {
        case 0: //Led
            pinMode(pin, OUTPUT);
            break;
        case 1: //button
            pinMode(pin, INPUT);
            break;
        case 2: //switchB
            pinMode(pin, INPUT);
            break;
        case 3: //dht
            DHT localDht(pin, dhtType);
            this->dht = localDht;
            this->dht.begin();
            delay(1000);
            break;
        case 4: //light
            pinMode(pin, INPUT);
            break;
        case 5: //battery
            break;
        case 6: //relay
            pinMode(pin, OUTPUT);
            break;
    }
}

void InputOutputManager::get()
{
    switch (inputOutputType) {
        case 0: //Led
            break;
        case 1: //button         
            break;
        case 2: //switchB   
            break;
        case 3: //dht
            int dhtvalues[2];
            dhtvalues[0] = (int)dht.readHumidity();
            dhtvalues[1] = (int)dht.readTemperature();
            return dhtvalues;
            break;
        case 4: //light
            return digitalRead(pin);
            break;
        case 5: //battery 
            double adcValue = analogRead(pin);
            int maxPinResolution = 1024;
            int maxPinVoltage = 3300;
            int voltageDivider = 2;
            adcValue = (((maxPinVoltage / maxPinResolution) * adcValue) * voltageDivider) / 1000;
            return adcValue;     
            break;
        case 6: //relay
            break;
    }
}

void InputOutputManager::set()
{
    switch (inputOutputType) {
        case 0: //Led
            break;
        case 1: //button         
            break;
        case 2: //switchB   
            break;
        case 3: //dht
            break;
        case 4: //light
            break;
        case 5: //battery    
            break;
        case 6: //relay
            break;
    }
}