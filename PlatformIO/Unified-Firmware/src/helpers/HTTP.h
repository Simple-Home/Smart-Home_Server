HTTPClient https;
String response;

bool sendHttpRequest(String requestJson, String token)
{
  client.setInsecure();
  https.setTimeout(15000);
  https.begin(client, String(apiHost + apiUrl));
  https.setReuse(true);
#ifdef ARDUINO_ESP8266_ESP_WROOM_02
  https.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
#else
  https.setFollowRedirects(2);
#endif
  https.addHeader("Content-Type", "application/json");

  int httpsCode = https.POST(requestJson);
#ifdef ENABLE_SERIAL_PRINT
  Serial.print("<- HTTP - " + (String)httpsCode);
#endif
  if (httpsCode > 0)
  {
#ifdef ENABLE_SERIAL_PRINT
    Serial.println(" - " + https.getString());
#endif
    jsonError = deserializeJson(jsonObject, https.getStream());
    if (jsonError.code() == DeserializationError::Ok)
    {
      https.end();
      return true;
    }
#ifdef ENABLE_SERIAL_PRINT
    Serial.println("Json Deserialize Error: " + String(jsonError.c_str()));
#endif
  }
  https.end();
  return false;
}

bool sendData(DynamicJsonDocument requestJson, String token)
{
  requestJson["token"] = token;
  String jsonString = "";
  serializeJson(requestJson, jsonString);
#ifdef ENABLE_SERIAL_PRINT
  Serial.println("-> " + jsonString);
#endif
  sendHttpRequest(jsonString, token);
  return false;
}
