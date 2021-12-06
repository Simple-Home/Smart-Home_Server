#include "config.h"
#include <Arduino.h>
#include <ArduinoJson.h>

#include "classes/EepromManager.h"
#include "classes/WifiManager.h"
#include "classes/HttpManager.h"
#include "classes/OutputManager.h"
#include "classes/DHTManager.h"
#include "classes/WebPageManager.h"

#include "functions/commands.h"
#include "functions/requests.h"

EepromManager eeprom_storage;
WifiManager wifi_conection;
OutputManager led(LED_BUILTIN);
DHTManager dht(DHT_PIN);
WebPageManager web_page("FILIP_ENVIRONMENT_0", "uQvN4pJnVb", eeprom_storage);
bool cannotConnect = false;


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
    web_page.Active();
    configurationReq(eeprom_storage.read(65, 97), wifi_conection);
  } else {
    cannotConnect = true;
    web_page.StartPage();
  }
}

void loop()
{
  //translate server to IP and port
  //send diag to server
  //comunication ower https
  if (cannotConnect) {
    if (wifi_conection.check(1)) {
      cannotConnect = false;
    }
    web_page.StartPage();
  } else {
    while (wifi_conection.check(30))
    {
      web_page.Active();
      DynamicJsonDocument doc = runtimeReq(eeprom_storage.read(65, 97));
      if (doc["command"]) {
        commandHandler(doc["command"], eeprom_storage);
      }
      if (doc["values"]) {
        led.on();
      }
    }
  }
}

