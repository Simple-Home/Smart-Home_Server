#include "config.h"
#include "DHTManager.h"
#include "InputManager.h"

DHTManager::DHTManager(byte pin, DHTType dhtType) : InputManager(){
    this->pin = pin;
    DHT dht(pin, dhtType);
    this->dht = dht;
    this->dht.begin();
    delay(1000);
};

byte get(byte get){
    if (get == (byte)1) {
        return (byte)this->dht.readHumidity();
    } else {
        return (byte)this->dht.readTemperature();
    }
}