#include <ArduinoJson.h>
#include "classes/HttpManager.h"

void configurationReq(String token){
  //Maybe to separet Function or class
  HttpManager http_conection((char *)"https://dev.steelants.cz", (char *)"443", (char *)"/vasek/home-update/api/v2", token);
  if (http_conection.connect((char *)"/endpoint/cofiguration"))
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

DynamicJsonDocument runtimeReq(String token){
  //Maybe to separet Function or class
  HttpManager http_conection((char *)"https://dev.steelants.cz", (char *)"443", (char *)"/vasek/home-update/api/v2", token);
  if (http_conection.connect((char *)"/endpoint"))
  {
    String body = "[{\"temp\":"dht.get(0)"},{\"humi\":"dht.get(1)"}]"
    http_conection.send((char *)"{\"value\":\"tests\"}");
    String payload = http_conection.getPayload();
    
    //json Deserialize test
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    http_conection.disconect();
    return doc;
  }
  delay(600);
}