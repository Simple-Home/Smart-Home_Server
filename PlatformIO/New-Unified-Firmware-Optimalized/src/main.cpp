#include "config.h"
#include <Arduino.h>

#include "classes/EepromManager.h"
#include "classes/WifiManager.h"
#include "classes/HttpManager.h"

EepromManager eeprom_storage;
WifiManager wifi_conection;

void setup()
{
  #ifdef ENABLE_SERIAL_PRINT
    Serial.begin(115200);
    while (!Serial)
      continue;
  #endif
  delay(1000);
  if (true) {
    eeprom_storage.write((char*)WIFI_SSID, 1, 33);
    eeprom_storage.write((char*)WIFI_PASS, 33, 65);
    eeprom_storage.save();
  }
}

void loop() {
  wifi_conection.connect(eeprom_storage.read(1,33),eeprom_storage.read(33,65));
  //translate server to IP and port
  //send diag to server
  //comunication ower https

  while (wifi_conection.check(30))
  {
    String token = eeprom_storage.read(65, 97);
    HttpManager http_conection((char*)"https://dev.steelants.cz", (char*)"", (char*)"/vasek/home-milanin/api/v2/endpoint", token);
    if (http_conection.connect()){
      http_conection.send((char*)"test");
      String payload = http_conection.getPayload();
      http_conection.disconect();
    }
    delay(600);
  }
}