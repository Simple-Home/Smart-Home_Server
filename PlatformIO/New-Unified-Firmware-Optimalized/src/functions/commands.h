#include <ArduinoJson.h>
#include "classes/EepromManager.h"

void commandHandler(String device_command, EepromManager eeprom){
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
    eeprom.erase();
    eeprom.save();
  } else {
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Command-Unknown");
    #endif
  }
}
