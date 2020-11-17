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
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Command - Config");
    #endif
    CleanEeprom();
    EEPROM.commit();
    ESP.restart();
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