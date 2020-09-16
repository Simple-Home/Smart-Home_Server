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
    Serial.println("No FP");
  }

  ESPhttpUpdate.setLedPin(LED_PIN, LOW);
  t_httpUpdate_return result = ESPhttpUpdate.update(UpdateClient, otaHost + otaUrl);

  switch (result)
  {
  case HTTP_UPDATE_FAILED:
#ifdef ENABLE_SERIAL_PRINT
    Serial.println("OTA - Update failed:");
    Serial.println("  LastError: " + ESPhttpUpdate.getLastError());
    Serial.println("  Error: " + ESPhttpUpdate.getLastErrorString());
#endif
    addLog("HTTP_UPDATE_FAILD Error (" + (String)ESPhttpUpdate.getLastError() + ") : " + (String)ESPhttpUpdate.getLastErrorString().c_str());
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