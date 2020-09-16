#include <config.h>
#include <config_adv.h>

//type Conversions
const char *stringToCharArray(String Text)
{
  char charBuf[] = "";
  Text.toCharArray(charBuf, Text.length());
  return charBuf;
}

//RAM :D
int waity = 0;
String ssid = "";
String pasw = "";
String apiToken = "";
#ifdef  ENABLE_SERVER_LOGS
  String logs = "";
#endif
#ifdef  SWITCH1_PIN
  bool state = false;
  bool buttonPushed = false;
#endif

StaticJsonDocument<265> jsonObject;
DeserializationError jsonError;
WiFiClientSecure client;

#ifdef DHT_PIN
  DHT dht(DHT_PIN, DHT_TYPLE);
#endif
#ifdef WIFI_CONFIG_PAGE
  ESP8266WebServer server(80);
  const byte DNS_PORT = 53;
  DNSServer dnsServer;
#endif

//Variables
const char apiCA[] PROGMEM = ""; //Fingerprint of Server Certificate
String apiHost = "https://dev.steelants.cz";
String apiUrl = "/vasek/home-update/api/endpoint";

#ifdef ENABLE_OTA
  //Simpe-Home OTA
  const char otaCA[] PROGMEM = ""; //Fingerprint of Server Certificate
  String otaHost = "https://dev.steelants.cz";
  String otaUrl = "/vasek/home-update/api/update";
#endif

#ifdef WIFI_CONFIG_PAGE
  //Configuration AP
  String configApName = CONFIG_AP_SSID;
  String configApPassword = CONFIG_AP_PASSWOR;
#endif

#include <functions/Utils.h>
#include <helpers/EEPROM.h>
#include <functions/Senzors.h>
#include <helpers/HTTP.h>
#include <helpers/Logs.h>
#include <helpers/OTA.h>
#include <helpers/Commands.h>
#include <helpers/Wifi.h>

void setup()
{
  EEPROM.begin(100);

  #ifdef ENABLE_SERIAL_PRINT
    Serial.begin(115200);
    while (!Serial)
      continue;
    delay(2000);
    Serial.println("Booted-UP");
  #endif

  #ifndef USE_EPRROM_WIFI_SETING
    CleanEeprom();
    WriteEeprom(WIFI_SSID, 1);
    WriteEeprom(WIFI_PASSWORD, 33);
    WriteEeprom(API_TOKEN, 65);
  #endif

  //read saved data
  ssid = ReadEeprom(1, 33);
  pasw = ReadEeprom(33, 65);
  apiToken = ReadEeprom(65, 97);

  //set pins
  #ifdef LIGHT_PIN
    pinMode(LIGHT_PIN, INPUT);
  #endif
  #ifdef PIR_PIN
    pinMode(PIR_PIN, INPUT);
  #endif
  //Wifi Conection
  if (!wifiConnect(ssid, pasw, true))
  {
    #ifdef WIFI_CONFIG_PAGE
        serveConfigPage();
    #endif
    return;
  }

  //Check OTA Updates
  #ifdef ENABLE_OTA
    otaHandler();
    #ifdef ENABLE_SERVER_LOGS
      sendLogs();
    #endif
  #endif

  //Diag Data sendData
  StaticJsonDocument<250> jsonContent = {};
  jsonContent["token"] = apiToken;
  jsonContent["settings"]["network"]["ip"] = WiFi.localIP().toString();
  jsonContent["values"]["wifi"]["value"] = (long)WiFi.RSSI();
  jsonContent["values"]["wifi"]["unit"] = "dBm";

  #ifdef ENABLE_SERIAL_PRINT
    Serial.println("MD5 Hash: " + ESP.getSketchMD5());
    Serial.println("Local IP: " + WiFi.localIP().toString());
    Serial.println("Mac: " + WiFi.macAddress());
  #endif
  sendData(jsonContent);
}
void loop()
{
  if (!waitForWifi(1))
  {
    #ifdef WIFI_CONFIG_PAGE
        dnsServer.processNextRequest();
        server.handleClient();
    #endif

    waity++;
    if (waity > WIFI_RECONNECT_INTERVAL)
    {
      waity = 0;
      wifiConnect(ssid, pasw);
    }
    delay(1);
    return;
  }

  StaticJsonDocument<250> jsonContent = {};
  jsonContent["token"] = apiToken;
  #ifdef DHT_PIN
    jsonContent["values"]["humi"]["value"] = (int)readTemperature(dht);
    jsonContent["values"]["humi"]["unit"] = "";
    jsonContent["values"]["temp"]["value"] = (int)readHumidity(dht);
    jsonContent["values"]["temp"]["unit"] = "";
  #endif
  #ifdef LIGHT_PIN
    jsonContent["values"]["light"]["value"] = readLight();
    jsonContent["values"]["light"]["unit"] = "";
  #endif
  #ifdef PIR_PIN
    jsonContent["values"]["move"]["value"] = readMovement();
    jsonContent["values"]["move"]["unit"] = "";
  #endif

  if (!sendData(jsonContent))
  {
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("REQ Failed");
    #endif
    return;
  }

  if (!jsonObject.containsKey("state") && !jsonObject.containsKey("values"))
  {
    return;
  }

  if (jsonObject["state"] != "succes")
  {
    return;
  }

  if (jsonObject.containsKey("command"))
  {
    commandExecution(jsonObject["command"]);
  }

  if (!jsonObject["device"]["hostname"])
  {
    WiFi.hostname(stringToCharArray(jsonObject["device"]["hostname"]));
  }
}