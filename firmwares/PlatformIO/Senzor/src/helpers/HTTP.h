HTTPClient https;
String response;

String sendHttpRequest(String requestJson, String token)
{
  client.setInsecure();
  https.begin(client, String(apiHost + apiUrl));
  https.setReuse(true);
  https.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);

  https.setRedirectLimit(1);
  https.addHeader("Content-Type", "application/json");

  int httpsCode = https.POST(requestJson);
  String payload = "";
  Serial.println(httpsCode);
  if (httpsCode > 0)
  {
    payload = https.getString();
    Serial.print(payload);
  }
  https.end();
  return payload;
}
bool sendData(StaticJsonDocument<250> requestJson, String token)
{
  requestJson["token"] = token;
  String jsonString = "";
  serializeJson(requestJson, jsonString);
  //Serial.println(jsonString);
  String response = sendHttpRequest(jsonString, token);

  if (response.length() > 1)
  {
    jsonError = deserializeJson(jsonObject, response);
    if (jsonError.code() == DeserializationError::Ok)
      return true;
  }
  return false;
}
