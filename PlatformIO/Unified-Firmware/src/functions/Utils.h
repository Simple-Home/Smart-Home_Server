const char *stringToCharArray(String Text)
{
  char charBuf[Text.length() + 1];
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

#if !defined(RELAY1_PIN) && !defined(ROTARY_ENC_PIN_1) && !defined(ROTARY_ENC_PIN_2)  && !defined(DEEP_SLEEP) 
  void setSleepTime(int minutes)
  {
    if (minutes > 0)
    {
    #if !defined(RELAY1_PIN) && !defined(ROTARY_ENC_PIN_1) && !defined(ROTARY_ENC_PIN_2)
      #ifdef ENABLE_SERIAL_PRINT
        Serial.println("going to sleep for: " + String(minutes * 60000) + "ms / " + String(minutes) + " minutes");
      #endif
      #ifdef DEEP_SLEEP
          ESP.deepSleep(minutes * 60000000);
      #else
          delay(minutes * 60000);
      #endif
    #endif
    }
  }
#endif
