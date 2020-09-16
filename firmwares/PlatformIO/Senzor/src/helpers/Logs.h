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
bool sendLogs()
{
  if (logs != "")
  {
    StaticJsonDocument<250> jsonContent = {};
    deserializeJson(jsonContent, "{\"logs\":[" + logs + "]}");
    jsonContent["token"] = apiToken;
    return sendData(jsonContent);
  }
  return false;
}
#endif