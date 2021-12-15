void WriteEeprom(String data, int start = 1, bool force = false)
{
  for (int i = 0; i < (int)data.length(); ++i)
  {
    EEPROM.write(start + i, data[i]);
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Writing EEPROM" + data[i]);
    #endif
    delay(100);
  }
  if (force == true){
    EEPROM.commit();
  }
}

void CleanEeprom(int plus = 0, bool force = false)
{
  for (int i = 1; i < (98 + plus); ++i)
  {
    EEPROM.write(i, ' ');
  }
  if (force == true){
    EEPROM.commit();
  }
}

String ReadEeprom(int min, int max)
{
  String localString;
  char loadString;
  for (int i = min; i < max; ++i)
  {
    loadString = char(EEPROM.read(i));
    if (loadString == ' ')
      break;
    if (loadString == 0)
      break;
    if (loadString == '�')
      break;
    localString += loadString;
  }
  return localString;
}

void SaveEeprom(){
  bool status = EEPROM.commit();
  #ifdef ENABLE_SERIAL_PRINT
    Serial.println((status) ? "EEPROM - Commit OK" : "EEPROM - Commit failed");
  #endif
  //EEPROM.end();
}