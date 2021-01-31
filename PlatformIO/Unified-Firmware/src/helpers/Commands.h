void commandExecution(String command, String token)
{
  if (command == "reset")
  {
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Command - Reset");
    #endif
    ESP.reset();
  }
  else if (command == "config")
  {
    #ifndef ENCODE_WIFI_SETING_TO_CODE
      #ifdef ENABLE_SERIAL_PRINT
          Serial.println("Command - Config");
      #endif

      //Delete Persistent WifiSetting
      WiFi.persistent(false);
      ESP.flashEraseSector(0x3fe);

      CleanEeprom(145);
      SaveEeprom();
      ESP.reset();
    #else
      #ifdef ENABLE_SERIAL_PRINT
          Serial.println("EEPROM - not supported with Hardcodet credential");
          Serial.println("EEPROM - uncoment ENCODE_WIFI_SETING_TO_CODE");
      #endif
    #endif
  }
  else if (command == "update")
  {
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Command - Update");
    #endif
    #ifdef ENABLE_OTA
      otaHandler();
    #endif
    #ifdef ENABLE_SERVER_LOGS
      sendLogs(token);
    #endif
  }
  else if (command == "null")
  {

  }
  else
  {
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Command - Unknown");
    #endif
  }
}