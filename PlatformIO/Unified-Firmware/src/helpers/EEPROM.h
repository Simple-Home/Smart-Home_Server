void WriteEeprom(String data, int start = 1)
{
  for (int i = 0; i < (int)data.length(); ++i)
  {
    EEPROM.write(start + i, data[i]);
  }
  EEPROM.commit();
}

void CleanEeprom(int plus = 0, bool force = false)
{
  for (int i = 1; i < (98 + plus); ++i)
  {
    EEPROM.write(i, 0);
  }
  if (true){
    EEPROM.commit();
  }
}

String ReadEeprom(int min, int max)
{
  String localString;
  for (int i = min; i < max; ++i)
  {
    if (EEPROM.read(i) == 0)
      break;
    localString += char(EEPROM.read(i));
  }
  return localString;
}