#include <Arduino.h>
#include "config.h"
#include "classes/Eeprom.h"

Eeprom epprom_storage();

void setup() {
    if (false){
      epprom_storage.erase();
      epprom_storage.write("1",1);
      epprom_storage.save();
    }
}

void loop() {
  //connect wifi
  //translate server to IP and port
  //send diag to server
  //comunication ower https

  while (/* wifi conected */)
  {
    /* loop */
  }
}