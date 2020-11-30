#ifdef ENABLE_SERVER_LOGS
  void addLog(String logMsg)
  {
    if (logs == "")
    {
      logs = "\"" + logMsg + "\"";
    }
    else
    {
      logs += ",\"" + logMsg + "\"";
    }
  }
  bool sendLogs(String apiToken)
  {
    if (logs != "")
    {
      StaticJsonDocument<290> jsonContent = {};
      jsonError = deserializeJson(jsonContent, "{\"logs\":[" + logs + "]}");
      if (jsonError.code() == DeserializationError::Ok){
        return sendData(jsonContent, apiToken);
      }
      #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Json deserializeJson Error: " + String(jsonError.c_str()));
      #endif
    }
    return false;
  }
#endif