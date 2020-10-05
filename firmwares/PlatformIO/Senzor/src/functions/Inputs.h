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

#ifdef LIGHT_PIN
    bool readLight() {
    return digitalRead(LIGHT_PIN);
    }
#endif


#ifdef BATT_SENSE
    double readBattery() {
        double adcValue = analogRead(BATT_SENSE);
        int maxPinResolution = 1024;
        int maxPinVoltage = 3300;
        int voltageDivider = 2;
        adcValue = (((maxPinVoltage / maxPinResolution) * adcValue) * voltageDivider) / 1000;
        return adcValue;
    }
#endif