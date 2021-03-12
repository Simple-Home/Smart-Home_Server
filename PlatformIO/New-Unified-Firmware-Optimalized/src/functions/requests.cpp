void configurationReq(){
  //Maybe to separet Function or class
  HttpManager http_conection((char *)"https://dev.steelants.cz", (char *)"443", (char *)"/vasek/home-update/api/v2", eeprom_storage.read(65, 97));
  if (http_conection.connect("/endpoint/cofiguration"))
  {
    http_conection.send((char *)"");
    String payload = http_conection.getPayload();
    
    //json Deserialize test
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    http_conection.disconect();
  }
  delay(600);
}

void runtimeReq(){
  //Maybe to separet Function or class
  HttpManager http_conection((char *)"https://dev.steelants.cz", (char *)"443", (char *)"/vasek/home-update/api/v2", eeprom_storage.read(65, 97));
  if (http_conection.connect("/endpoint"))
  {
    http_conection.send((char *)"{\"value\":\"tests\"}");
    String payload = http_conection.getPayload();
    
    //json Deserialize test
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    if (doc["command"]) {
      commandHandler(doc["command"]);
    }

    if (doc["values"]) {
      led.on();
    }

    http_conection.disconect();
  }
  delay(600);
}