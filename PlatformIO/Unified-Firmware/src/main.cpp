#include <config.h>
#include <config_adv.h>

//RAM :D
int waity = 0;
String ssid = "";
String pasw = "";
String apiToken = "";
bool wifiOnce = true;
#ifdef  ENABLE_SERVER_LOGS
  String logs = "";
#endif
#ifdef  RELAY1_PIN
  bool state = false;
  bool buttonPushed = false;
  volatile unsigned long last_micros = 0;
#endif
  volatile unsigned long last_micros_wifi = 0;
#if defined(ROTARY_ENC_PIN_1) && defined(ROTARY_ENC_PIN_2)
  float lastTemperature = 0;
#endif
#ifdef STATIC_IP_SUPPORT
    String staticIP = "";
    String gateway = "";
    String subnet = "";
#endif
DynamicJsonDocument jsonObject(290);
DeserializationError jsonError;
WiFiClientSecure client;

#ifdef DHT_PIN
  DHT dht(DHT_PIN, DHT_TYPLE);
#endif
#ifdef WIFI_CONFIG_PAGE
  bool configPage = false;
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
String apiHost = API_HOST;
String apiUrl = API_URL;

#ifdef ENABLE_OTA
  //Simpe-Home OTA
  const char otaCA[] PROGMEM = ""; //Fingerprint of Server Certificate
  String otaHost = OTA_HOST;
  String otaUrl = OTA_URL;
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

void sendDiag(){
  DynamicJsonDocument jsonContent(259);
  jsonContent["settings"]["network"]["ip"] = WiFi.localIP().toString();
  jsonContent["settings"]["network"]["mac"] = WiFi.macAddress();
  jsonContent["settings"]["firmware_hash"] = ESP.getSketchMD5();
  jsonContent["values"]["wifi"]["value"] = (long)WiFi.RSSI();
  jsonContent["values"]["wifi"]["unit"] = "dBm";
  sendData(jsonContent, apiToken);
  if (jsonObject.containsKey("command"))
  {
    commandExecution(jsonObject["command"], apiToken);
  }
}

void setup()
{
  #ifdef ENABLE_SERIAL_PRINT
    Serial.begin(115200);
    while (!Serial) continue;
    delay(2000);
    Serial.println("Booted-UP");
  #endif

  #ifdef DHT_PIN
    dht.begin();
    delay(1000);
  #endif

  EEPROM.begin(145);

  #ifdef ENCODE_WIFI_SETING_TO_CODE
    CleanEeprom(true);
    WriteEeprom(WIFI_SSID, 1);
    WriteEeprom(WIFI_PASSWORD, 33);
    WriteEeprom(API_TOKEN, 65);
    #ifdef STATIC_IP_SUPPORT
      //délka jené položky je 15
      WriteEeprom(STATIC_IP, 97);
      WriteEeprom(GATEWAY, 113);
      WriteEeprom(SUBNET, 129);
    #endif
  #endif

  //read saved data
  ssid = ReadEeprom(1, 33);
  pasw = ReadEeprom(33, 65);
  apiToken = ReadEeprom(65, 97);
  #ifdef STATIC_IP_SUPPORT
    //délka jené položky je 15
    staticIP = ReadEeprom(97, 113);
    gateway = ReadEeprom(113, 129);
    subnet = ReadEeprom(129, 145);
  #endif

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
  #if defined(ROTARY_ENC_PIN_1) && defined(ROTARY_ENC_PIN_2)
    pinMode(ROTARY_ENC_PIN_1, OUTPUT);
    pinMode(ROTARY_ENC_PIN_2, OUTPUT);
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
  #ifdef STATIC_IP_SUPPORT
    if (!setStaticIP(ssid, pasw, staticIP, gateway, subnet))
    {
      #ifdef WIFI_CONFIG_PAGE
          serveConfigPage();
      #endif
      delay(1000);
      return;
    }
  #else
    if (!wifiConnect(ssid, pasw, true) || apiToken == "")
    {
      #ifdef WIFI_CONFIG_PAGE
          serveConfigPage();
          #ifdef ENABLE_SERIAL_PRINT
            Serial.println("Config page starting from SETUP()");
          #endif
      #endif
      delay(1000);
      return;
    }
  #endif

  #ifdef ENABLE_SERIAL_PRINT
    Serial.println("Local IP: " + WiFi.localIP().toString());
    Serial.println("Mac: " + WiFi.macAddress());
    Serial.println("MD5 Firmware Hash: " + ESP.getSketchMD5());
  #endif

  //Check OTA Updates
  #ifdef ENABLE_OTA
    otaHandler();
    #ifdef ENABLE_SERVER_LOGS
      sendLogs(apiToken);
    #endif
  #endif

  //Send Diag Data to Server
  sendDiag();
}
void loop()
{
  #ifdef WATCHDOG_TRASHOLD
    watchdog.watch();
  #endif
  if (!waitForWifi(1))
  {
    #ifdef WIFI_CONFIG_PAGE
        if (configPage) {
          configPage = false;
          serveConfigPage();
        }
        dnsServer.processNextRequest();
        server.handleClient();
    #endif
    
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println(String(configPage) + "last time reconecr happen" + String(((long)millis() - last_micros_wifi)));
      Serial.println(String(configPage) + "last time reconecr happen" + String(WIFI_RECONNECT_INTERVAL));
    #endif

    if (((long)millis() - last_micros_wifi) >= WIFI_RECONNECT_INTERVAL) {
      wifiConnect(ssid, pasw);
      #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Conecting Back to WI-FI");
      #endif
      last_micros_wifi = millis();
      delay(1);
    }
    return;
        Serial.println("test");

  } else {
    last_micros_wifi = 0;
  }

  DynamicJsonDocument jsonContent(259);
  #ifdef SWITCH1_PIN
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
    jsonContent["values"]["humi"]["value"] = (int)dht.readHumidity();
    jsonContent["values"]["humi"]["unit"] = "%";
    jsonContent["values"]["temp"]["value"] = (int)dht.readTemperature();
    jsonContent["values"]["temp"]["unit"] = "C";
  #endif
  #ifdef LIGHT_PIN
    jsonContent["values"]["light"]["value"] = readLight();
    jsonContent["values"]["light"]["unit"] = "";
  #endif
  #ifdef PIR_PIN
    jsonContent["values"]["move"]["value"] = readMovement();
    jsonContent["values"]["move"]["unit"] = "";
  #endif
  #ifdef BATT_SENSE
    jsonContent["values"]["battery"]["value"] = readBattery();
    jsonContent["values"]["battery"]["unit"] = "v";
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
    if ((long)(millis() - last_micros) >= SWITCH1_DEBOUNCE_INTERVAL)
    {
      bool serverState = jsonObject["values"]["on/off"];
      if (serverState != state)
      {
        if (buttonPushed)
        {
          return;
        }
        SetRelayState(serverState);
      }
    }
  #endif

  #if defined(ROTARY_ENC_PIN_1) && defined(ROTARY_ENC_PIN_2)
      int temperature = jsonObject["values"]["temp_cont"];
      if (lastTemperature != temperature) {
        setTemperature(temperature);
        lastTemperature = temperature;
      }
  #endif

  if (jsonObject.containsKey("command"))
  {
    commandExecution(jsonObject["command"], apiToken);
  }

  if (jsonObject["device"]["hostname"])
  {
    WiFi.hostname(stringToCharArray(jsonObject["device"]["hostname"]));
  }

  if (jsonObject["device"]["sleepTime"])
  {
    int minutes = jsonObject["device"]["sleepTime"];
    if (minutes > 0) {
      #ifdef ENABLE_SERIAL_PRINT
          Serial.println("going to sleep for: " + String(minutes * 60000) +  "ms / " + String(minutes) + " minutes" );
      #endif
      #if !defined(RELAY1_PIN) && !defined(ROTARY_ENC_PIN_1) && !defined(ROTARY_ENC_PIN_2)
        #ifdef DEEP_SLEEP
          ESP.deepSleep(minutes * 60000000);
        #else
          delay(minutes * 60000);
        #endif
      #endif
    }
  }
}