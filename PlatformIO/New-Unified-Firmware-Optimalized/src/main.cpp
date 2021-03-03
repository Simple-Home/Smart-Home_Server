#include <Arduino.h>
#include "config.h"
#include "classes/Eeprom.h"

Eeprom memory();

void setup() {
    if (false){
      memory.erase();
      memory.write("1",1);
      memory.save();
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