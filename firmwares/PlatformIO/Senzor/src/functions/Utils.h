const char *stringToCharArray(String Text)
{
  char charBuf[Text.length()];
  Text.toCharArray(charBuf, Text.length());
  return charBuf;
}

#ifdef LED_PIN
  void ledWaiting()
  {
    digitalWrite(LED_PIN, LOW);
    delay(500);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
  }
#endif

bool Contains(String s, String search)
{
  int max = s.length() - search.length();

  for (int i = 0; i <= max; i++)
  {
    if (s.substring(i) == search)
      return true; // or i
  }

  return false; //or -1
}

