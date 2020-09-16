#ifdef RELAY1_PIN
    void SetRelayState(bool relayState)
    {
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println("Changing relay state -> " + String(relayState));
        #endif
        digitalWrite(RELAY1_PIN, relayState);
        EEPROM.write(0, relayState);
        EEPROM.commit();
        state = relayState;
    }
    void SetRelayLastState()
    {
        bool relayState = EEPROM.read(0);
        #ifdef ENABLE_SERIAL_PRINT
            Serial.println("Changing relay state -> " + String(relayState));
        #endif
        digitalWrite(RELAY1_PIN, relayState);
        state = relayState;
    }
    void ICACHE_RAM_ATTR handleInterruptFalling()
    {
        if ((long)(millis() - last_micros) >= SWITCH1_DEBOUNCE_INTERVAL) //check at home
        {
            buttonPushed = true;
            SetRelayState(!state);
            last_micros = millis();
        }
    }
#endif