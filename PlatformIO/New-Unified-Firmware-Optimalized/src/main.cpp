#include "config.h"
#include <Arduino.h>
#include <ArduinoJson.h>

#include "classes/EepromManager.h"
#include "classes/WifiManager.h"
#include "classes/HttpManager.h"
#include "classes/OutputManager.h"

#include "functions/commands.cpp"
#include "functions/requests.cpp"

EepromManager eeprom_storage;
WifiManager wifi_conection;
OutputManager led(LED_BUILTIN);

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
    configurationReq(eeprom_storage.read(65, 97));
  }
}

void loop()
{
  //translate server to IP and port
  //send diag to server
  //comunication ower https

  while (wifi_conection.check(30))
  {
    DynamicJsonDocument doc = runtimeReq(eeprom_storage.read(65, 97));
    if (doc["command"]) {
      commandHandler(doc["command"], eeprom_storage);
    }
    if (doc["values"]) {
      led.on();
    }
  }
}

