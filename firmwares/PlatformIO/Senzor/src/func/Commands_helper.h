void commandExecution(String command)
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
    otaHandler();
#ifdef ENABLE_SERVER_LOGS
    sendLogs();
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