

class Watchdog
{
private:
    int watcher = 0;
    int trashold = 0;

public:
    Watchdog(int trasholdArg);
    void watch();
    void log();
    void reset();
};

Watchdog::Watchdog(int trasholdArg)
{
    this->trashold = trasholdArg;
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Watchdog - Trashhold Set: " + (String)trasholdArg + "/" + (String)watcher);
    #endif
}

void Watchdog::watch()
{
    if (this->watcher >= trashold)
    {
        #ifdef ENABLE_SERIAL_PRINT
                Serial.println("Watchdog - Restarting ESP");
        #endif
        ESP.restart();
    }
    return;
}

void Watchdog::reset()
{
    this->watcher = 0;
    return;
}

void Watchdog::log()
{
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Watchdog - Watch detected");
    #endif
    this->watcher++;
    return;
}
