#ifdef DHT_PIN
float readTemperature(DHT dhtSenzor)
{
    for (size_t i = 0; i < 4; i++)
    {
        delay(2000);
        float t = dhtSenzor.readTemperature();
        if (!isnan(t) && t != 2147483647)
        {
            return t;
        }
    }
    return 999;
}
float readHumidity(DHT dhtSenzor)
{
    for (size_t i = 0; i < 4; i++)
    {
        delay(2000);
        float h = dhtSenzor.readHumidity();
        if (!isnan(h) && h != 2147483647){
            return h;
        }
    }
    return 999;

}
#endif

#ifdef LIGHT_PIN
bool readLight()
{
    return digitalRead(LIGHT_PIN);
}
#endif

#ifdef BATT_SENSE
double readBattery()
{
    double adcValue = analogRead(BATT_SENSE);
    int maxPinResolution = 1024;
    int maxPinVoltage = 3300;
    int voltageDivider = 2;
    adcValue = (((maxPinVoltage / maxPinResolution) * adcValue) * voltageDivider) / 1000;
    return adcValue;
}
#endif