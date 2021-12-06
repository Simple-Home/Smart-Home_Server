#include <ArduinoJson.h>
#include "classes/HttpManager.h"

void configurationReq(String token){
    //Maybe to separet Function or class
    HttpManager http_conection((char *)"https://dev.steelants.cz", (char *)"443", (char *)"/vasek/simple-home-v4/public/api/v1", token);
    if (http_conection.connect((char *)"/setup"))
    {
        DynamicJsonDocument doc(1024);
        JsonObject root = doc.to<JsonObject>();

        #ifdef DHT_PIN
            root["properties"][0] = "temp";
            root["properties"][1] = "humi";
        #endif
        
        char body[256];
        serializeJson(doc, body);
        http_conection.send(body);
        String payload = http_conection.getPayload();
        
        //json Deserialize test
        deserializeJson(doc, payload);
        http_conection.disconect();
    }
    delay(600);
}

DynamicJsonDocument runtimeReq(String token){
    //Maybe to separet Function or class
    HttpManager http_conection((char *)"https://dev.steelants.cz", (char *)"443", (char *)"/vasek/simple-home-v4/public/api/v1", token);
    if (http_conection.connect((char *)"/data"))
    {
        DynamicJsonDocument doc(1024);
        JsonObject root = doc.to<JsonObject>();

        #ifdef DHT_PIN
            DHTManager dht(DHT_PIN);
            root["temp"] = dht.get(0);
            root["humi"] = dht.get(1);
        #endif
        
        char body[256];
        serializeJson(doc, body);
        http_conection.send(body);
        
        String payload = http_conection.getPayload();
        //json Deserialize test
        deserializeJson(doc, payload);
        http_conection.disconect();
        return doc;
    }
    delay(600);
}