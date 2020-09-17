#include <config.h>
#include <config_adv.h>

//RAM :D
int waity = 0;
String ssid = "";
String pasw = "";
String apiToken = "";
#ifdef  ENABLE_SERVER_LOGS
  String logs = "";
#endif
#ifdef  RELAY1_PIN
  bool state = false;
  bool buttonPushed = false;
  volatile unsigned long last_micros;
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
#ifdef WATCHDOG_TRASHOLD
  #include <helpers/Watchdog.h>
  Watchdog watchdog(WATCHDOG_TRASHOLD);
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
#include <functions/Outputs.h>
#include <functions/Inputs.h>
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
    while (!Serial) continue;
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
  #ifdef PIR_PIN
    pinMode(PIR_PIN, INPUT);
  #endif
  #ifdef LED_PIN
    pinMode(LED_PIN, OUTPUT);
  #endif
  #ifdef RELAY1_PIN
    pinMode(RELAY1_PIN, OUTPUT);
    #ifdef RELAY1_RECOVER_STATE_ON_POWER_LOSS
      SetRelayLastState();
    #else
      SetRelayState(false);
    #endif
  #endif
  #ifdef SWITCH1_PIN
    pinMode(SWITCH1_PIN, INPUT);
    #ifdef MOMENTARY_SWITCH
      attachInterrupt(SWITCH1_PIN, handleInterruptFalling, FALLING);
    #elif defined(ON_OFF_SWITCH)
      attachInterrupt(SWITCH1_PIN, handleInterruptFalling, RISING);
      attachInterrupt(SWITCH1_PIN, handleInterruptFalling, FALLING);
    #endif
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
      sendLogs(apiToken);
    #endif
  #endif

  //Diag Data sendData
  StaticJsonDocument<250> jsonContent = {};
  jsonContent["settings"]["network"]["ip"] = WiFi.localIP().toString();
  jsonContent["settings"]["network"]["mac"] = WiFi.macAddress();
  jsonContent["settings"]["firmware_hash"] = ESP.getSketchMD5();

  jsonContent["values"]["wifi"]["value"] = (long)WiFi.RSSI();
  jsonContent["values"]["wifi"]["unit"] = "dBm";

  #ifdef ENABLE_SERIAL_PRINT
    Serial.println("Local IP: " + WiFi.localIP().toString());
    Serial.println("Mac: " + WiFi.macAddress());
    Serial.println("MD5 Firmware Hash: " + ESP.getSketchMD5());
  #endif

  sendData(jsonContent, apiToken);
}
void loop()
{
  #ifdef WATCHDOG_TRASHOLD
    watchdog.watch();
  #endif
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
  #ifdef RELAY1_PIN
    if (buttonPushed)
    {
      jsonContent["values"]["wifi"]["value"] = (long)WiFi.RSSI();
      jsonContent["values"]["wifi"]["unit"] = "dBm";
      jsonContent["values"]["on/off"]["value"] = state;
      #ifdef ENABLE_SERIAL_PRINT
          Serial.println("Sending State to server - " + String(state));
      #endif
      buttonPushed = false;
    }
  #endif
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

  if (!sendData(jsonContent, apiToken))
  {
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("REQ Failed");
    #endif
    #ifdef WATCHDOG_TRASHOLD
      watchdog.log();
    #endif
    return;
  }

  if (!jsonObject.containsKey("state") && !jsonObject.containsKey("values") && jsonObject["state"] != "succes")
  {
    #ifdef WATCHDOG_TRASHOLD
      watchdog.log();
    #endif
    return;
  }
  #ifdef WATCHDOG_TRASHOLD
    watchdog.reset();
  #endif

  #ifdef RELAY1_PIN
    bool serverState = jsonObject["values"]["on/off"];
    if (serverState != state)
    {
      if (buttonPushed)
      {
        return;
      }
      SetRelayState(serverState);
    }
  #endif

  if (jsonObject.containsKey("command"))
  {
    commandExecution(jsonObject["command"], apiToken);
  }

  if (!jsonObject["device"]["hostname"])
  {
    WiFi.hostname(stringToCharArray(jsonObject["device"]["hostname"]));
  }
}