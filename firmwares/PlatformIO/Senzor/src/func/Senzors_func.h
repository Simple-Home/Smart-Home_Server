#ifdef DHT_PIN
    float readTemperature(DHT dhtSenzor){
    delay(1000);
    float t = dhtSenzor.readTemperature();
    if (isnan(t))  {
        return 999;
    } else {
        return t;
    }
    }
    float readHumidity(DHT dhtSenzor){
    delay(1000);
    float h = dhtSenzor.readHumidity();
    if (isnan(h))  {
        return 999;
    } else {
        return h;
    }
    }
#endif

#ifdef LIGHT_PIN
    bool readLight() {
    return digitalRead(LIGHT_PIN);
    }
#endif