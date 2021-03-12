#include "config.h"
#include <Arduino.h>
#include <ArduinoJson.h>

#include "classes/EepromManager.h"
#include "classes/WifiManager.h"
#include "classes/HttpManager.h"
#include "classes/OutputManager.h"

EepromManager eeprom_storage;
WifiManager wifi_conection;
OutputManager led(LED_BUILTIN);

void commandHandler(String device_command){
  if (device_command == "reset"){
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Command-Restarting");
    #endif
    ESP.reset();
  } else if (device_command == "refresh"){
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Command-RefreshSettings");
    #endif
  } else if (device_command =="update"){
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Command-Updating");
    #endif
  } else if (device_command == "config"){
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Command-Configuration");
    #endif
  } else if (device_command == "factory"){
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Command-FactryReset");
    #endif
    eeprom_storage.erase();
    eeprom_storage.save();
  } else {
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Command-Unknown");
    #endif
  }
}

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

void setup()
{
  #ifdef ENABLE_SERIAL_PRINT
    Serial.begin(115200);
    while (!Serial)
      continue;
  #endif
  delay(1000);
  led.off();

  wifi_conection.connect(eeprom_storage.read(1, 33), eeprom_storage.read(33, 65));
  if (wifi_conection.check(30)){
    configurationReq();
  }
}

void loop()
{
  //translate server to IP and port
  //send diag to server
  //comunication ower https

  while (wifi_conection.check(30))
  {
    runtimeReq();
  }
}

