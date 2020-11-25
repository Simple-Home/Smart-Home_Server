

    class Watchdog{
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
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println("Watchdog - Trashhold Set: " + (String) trasholdArg);
        #endif
        this->trashold = trasholdArg;
    }

    void Watchdog::watch()
    {
        if (this->watcher > trashold)
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
