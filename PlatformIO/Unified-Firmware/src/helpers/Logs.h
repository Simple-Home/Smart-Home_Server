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
      StaticJsonDocument<250> jsonContent = {};
      deserializeJson(jsonContent, "{\"logs\":[" + logs + "]}");
      return sendData(jsonContent, apiToken);
    }
    return false;
  }
#endif