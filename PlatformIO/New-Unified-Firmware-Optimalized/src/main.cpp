#include "config.h"
#include <Arduino.h>

#include "classes/EepromManager.h"
#include "classes/WifiManager.h"
#include "classes/HttpManager.h"

EepromManager eeprom_storage;
WifiManager wifi_conection;

void setup() {
  #ifdef ENABLE_SERIAL_PRINT
    Serial.begin(115200);
    while (!Serial) continue;
  #endif
  if (true) {
    eeprom_storage.write((WIFI_SSID).c_str(), 1);
    eeprom_storage.write((WIFI_PASS).c_str(), 33);
  }
}

void loop() {
  wifi_conection.connect(eeprom_storage.read(1,33), eeprom_storage.read(33, 65));
  //translate server to IP and port
  //send diag to server
  //comunication ower https

  while (wifi_conection.check())
  {
    char* token = eeprom_storage.read(65, 97);
    HttpManager http_conection(("https://dev.steelants.cz").c_str(), ("").c_str(), ("/vasek/home-milanin/api/v2/endpoint").c_str(), token);
    if (http_conection.connect()){
      http_conection.send("test");
      String payload = http_conection.getPayload();
      http_conection.disconect();
    }
  }
}