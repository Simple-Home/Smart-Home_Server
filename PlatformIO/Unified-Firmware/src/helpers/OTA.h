#ifdef ENABLE_OTA
  void otaHandler()
  {
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("OTA - Starting");
    #endif

    configTime(3 * 3600, 0, "pool.ntp.org");
    BearSSL::WiFiClientSecure UpdateClient;

    if (strlen(otaCA) > 0)
    {
      UpdateClient.setFingerprint(otaCA);
    }
    else
    {
      UpdateClient.setInsecure();
      #ifdef ENABLE_SERIAL_PRINT
        Serial.println("No FP");
      #endif
    }
    #ifdef LED_PIN
        ESPhttpUpdate.setLedPin(LED_PIN, LOW);
    #endif
    t_httpUpdate_return result = ESPhttpUpdate.update(UpdateClient, otaHost + otaUrl);

    switch (result)
    {
    case HTTP_UPDATE_FAILED:
      #ifdef ENABLE_SERIAL_PRINT
        Serial.println("OTA - Update failed:");
        Serial.println("  LastError: " + ESPhttpUpdate.getLastError());
        Serial.println("  Error: " + ESPhttpUpdate.getLastErrorString());
      #endif
      #ifdef ENABLE_SERVER_LOGS
        addLog("HTTP_UPDATE_FAILD Error (" + (String)ESPhttpUpdate.getLastError() + ") : " + (String)ESPhttpUpdate.getLastErrorString().c_str());
      #endif
      break;

    case HTTP_UPDATE_NO_UPDATES:
      #ifdef ENABLE_SERIAL_PRINT
          Serial.println("OTA - No Update Available");
      #endif
      break;

    case HTTP_UPDATE_OK:
      #ifdef ENABLE_SERIAL_PRINT
          Serial.println("OTA - Update OK");
      #endif
      break;
    }
  }
#endif