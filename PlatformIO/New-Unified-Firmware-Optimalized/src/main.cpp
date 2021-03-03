#include "config.h"
#include <Arduino.h>

#include "classes/EepromManager.h"
#include "classes/WifiManager.h"
#include "classes/HttpManager.h"

EepromManager eeprom_storage();
WifiManager wifi_conection();

void setup() {
  #ifdef ENABLE_SERIAL_PRINT
    Serial.begin(115200);
    while (!Serial) continue;
  #endif
  if (true) {
    eeprom_storage.write(SSID, 1);
    eeprom_storage.write(PASSWD, 33);
  }
}

void loop() {
  wifi_conection.connect(eeprom_storage.read(1,33), eeprom_storage.read(33, 65));
  //translate server to IP and port
  //send diag to server
  //comunication ower https

  while (wifi_conection.check())
  {
    token = eeprom_storage.read(65, 97);
    HttpManager http_conection("https://dev.steelants.cz", "", "/vasek/home-milanin/api/v2/endpoint", token);
    if (http_conection.connect()){
      String response = http_conection.send();
      //main loop
      http_conection.disconect();
    }
  }
}