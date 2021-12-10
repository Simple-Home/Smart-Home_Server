HTTPClient https;
String response;

String sendHttpRequest(String requestJson, String token)
{
  client.setInsecure();
  https.begin(client, String(apiHost + apiUrl));
  https.setReuse(true);

#ifdef ARDUINO_ESP8266_ESP_WROOM_02
  https.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
#else
  https.setFollowRedirects(2);
#endif

  https.setRedirectLimit(1);
  https.addHeader("Content-Type", "application/json");

  int httpsCode = https.POST(requestJson);
  String payload = "";
#ifdef ENABLE_SERIAL_PRINT
  Serial.print("<- HTTP - " + (String)httpsCode);
#endif
  if (httpsCode > 0)
  {
    payload = https.getString();
#ifdef ENABLE_SERIAL_PRINT
    Serial.println(" - " + payload);
#endif
  }
  https.end();
  return payload;
}

bool sendData(DynamicJsonDocument requestJson, String token)
{
    requestJson["token"] = token;
    String jsonString = "";
    serializeJson(requestJson, jsonString);
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("-> " + jsonString);
    #endif
    String response = sendHttpRequest(jsonString, token);

    if (response.length() > 1)
    {
      jsonError = deserializeJson(jsonObject, response);
      if (jsonError.code() == DeserializationError::Ok)
      {
        return true;
      }
      #ifdef ENABLE_SERIAL_PRINT
          Serial.println("Json Deserialize Error: " + String(jsonError.c_str()));
      #endif
    }
    return false;
}
