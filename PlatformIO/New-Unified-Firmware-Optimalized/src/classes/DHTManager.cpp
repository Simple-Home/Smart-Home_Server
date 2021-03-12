#include "config.h"
#include "DHTManager.h"
#include "Adafruit_Sensor.h"
#include "DHT.h"

byte DHTManager::get(byte get){
    if (get == (byte)1) {
         return (int)this->dht.readHumidity();
    } else {
        return (int)this->dht.readTemperature();
    }
    return get;
}