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

#if defined(ROTARY_ENC_PIN_1) && defined(ROTARY_ENC_PIN_2)
    void setTemperature(float temp) {
        int z = 0;
        // Set temperature to OFF
        for(uint8_t i = 0; i < 60; ++i) {
            digitalWrite(ROTARY_ENC_PIN_2, z);
            delay(30);
            digitalWrite(ROTARY_ENC_PIN_1, z);
            delay(30);

            z = (z+1) % 2;
        }

        /* Substract 4.5 from the given temperature to compute the number of steps
        * required to set the desired temperature, as the temperature directly jumps
        * from OFF tp 5C. */
        for(uint8_t i = 0; i < (temp-4.5)*2; ++i) {
            digitalWrite(ROTARY_ENC_PIN_1, z);
            delay(30);
            digitalWrite(ROTARY_ENC_PIN_2, z);
            delay(30);

            z = (z+1) % 2;
        }
    }
#endif
