#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <DNSServer.h>

//Pins
#define SONOFF_RELAY 12
#define SONOFF_LED 13
#define SONOFF_BUT 0

//type Conversions
const char *stringToCharArray(String Text)
{
  char charBuf[Text.length()];
  Text.toCharArray(charBuf, Text.length());
  return charBuf;
}

//RAM :D
int waity = 0;
String ssid = "";
String pasw = "";
String apiToken = "";
bool state = false;
bool buttonPushed = false;
StaticJsonDocument<265> jsonObject;
DeserializationError jsonError;
ESP8266WebServer server(80);
const byte DNS_PORT = 53;
DNSServer dnsServer;
WiFiClientSecure client;

//Variables
const char apiCA[] PROGMEM = "EB D7 60 4A 74 34 F3 97 AE 9C 74 75 52 66 AA 37 0E A1 90 D8"; //Fingerprint of Server Certificate
String apiHost = "https://dev.steelants.cz";
String apiUrl = "/vasek/home-update/api/endpoint";

//Simpe-Home OTA
const char otaCA[] PROGMEM = "EB D7 60 4A 74 34 F3 97 AE 9C 74 75 52 66 AA 37 0E A1 90 D8"; //Fingerprint of Server Certificate
String otaHost = "https://dev.steelants.cz";
String otaUrl = "/vasek/home/update.php";

//EPROM Functions
void WriteEeprom(String data, int start = 1)
{
  for (int i = 0; i < (int)data.length(); ++i)
  {
    EEPROM.write(start + i, data[i]);
  }
  EEPROM.commit();
}
void CleanEeprom()
{
  for (int i = 1; i < 100; ++i)
  {
    EEPROM.write(i, 0);
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

//State Functions
void SetRelayState(bool relayState)
{
  Serial.println("Changing relay state -> " + String(relayState));
  digitalWrite(SONOFF_RELAY, relayState);
  EEPROM.write(0, relayState);
  EEPROM.commit();
  state = relayState;
}
void SetRelayLastState()
{
  bool relayState = EEPROM.read(0);
  Serial.println("Changing relay state -> " + String(relayState));
  digitalWrite(SONOFF_RELAY, relayState);
  state = relayState;
}

//LOG Functions
void addLog(String logMsg)
{
}

//Utils Functions
void ledWaiting()
{
  digitalWrite(SONOFF_LED, LOW);
  delay(500);
  digitalWrite(SONOFF_LED, HIGH);
  delay(500);
}
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
void otaHandler()
{
  client.setInsecure();
  configTime(3 * 3600, 0, "pool.ntp.org");

  if (!client.connect(stringToCharArray(otaHost), 443))
  {
    addLog("connection failed");
    Serial.println("OTA - Connection Failed!");
    return;
  }

  if (otaCA != "" || !client.verify(stringToCharArray(otaCA), stringToCharArray(otaHost)))
  {
    addLog("certificate doesn't match");
    Serial.println("OTA - Server Certificate is not Valid!");
    return;
  }

  Serial.println("OTA - Starting Update");
  auto ret = ESPhttpUpdate.update(client, stringToCharArray(otaHost), 80, stringToCharArray(otaUrl));
  delay(500);

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    addLog("HTTP_UPDATE_FAILD Error (" + (String)ESPhttpUpdate.getLastError() + ") : " + (String)ESPhttpUpdate.getLastErrorString().c_str());
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
    break;
  }
  delay(500);
}
void commandExecution(String command)
{
  if (command == "reset")
  {
    Serial.println("Command - Reset");
    ESP.reset();
  }
  else if (command == "config")
  {
    Serial.println("Command - Config");
    CleanEeprom();
    EEPROM.commit();
    ESP.restart();
  }
  else if (command == "null")
  {
  }
  else
  {
    Serial.println("Command - Unknown");
  }
}

//HTTP Request Functions
HTTPClient https;
String response;

String sendHttpRequest(String requestJson)
{
  client.setInsecure();
  https.begin(client, String(apiHost + apiUrl));
  https.setReuse(true);
  https.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);

  https.setRedirectLimit(1);
  https.addHeader("Content-Type", "application/json");

  int httpsCode = https.POST(requestJson);
  String payload = "";
  Serial.println(httpsCode);
  if (httpsCode > 0)
  {
    payload = https.getString();
    Serial.print(payload);
  }
  https.end();
  return payload;
}
bool sendData(StaticJsonDocument<250> requestJson)
{
  String jsonString = "";
  serializeJson(requestJson, jsonString);
  //Serial.println(jsonString);
  String response = sendHttpRequest(jsonString);

  if (response.length() > 1)
  {
    jsonError = deserializeJson(jsonObject, response);
    if (jsonError.code() == DeserializationError::Ok)
      return true;
  }
  return false;
}

//WiFi Functions
bool waitForWifi(int timeout = 30)
{
  int i = 0;
  Serial.println("Waiting for Wifi");
  while (i < timeout)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    ledWaiting();
    Serial.println("Connecting.. status: " + String(WiFi.status()));
    i++;
  }
  return false;
}
bool wifiConnect(String localSsid, String localPasw, bool waitUntilConnect = false)
{
  WiFi.persistent(false);
  Serial.print("SSID:");
  Serial.print(localSsid);
  Serial.println(":");

  Serial.print("Password:");
  Serial.print(localPasw);
  Serial.println(":");

  WiFi.mode(WIFI_STA);
  WiFi.begin(localSsid, localPasw);
  if (waitUntilConnect)
  {
    waitForWifi(30);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected!");
    return true;
  }

  Serial.println("Unable to connect!" + WiFi.status());
  return false;
}
String wifiScan()
{
  String wifiHtmlList = "";
  int n = WiFi.scanNetworks();
  Serial.println("Wifi scan Done");
  if (n == 0)
  {

    Serial.println("no networks found");
    wifiHtmlList += "<label>No networks found...</label>";
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      wifiHtmlList += "<a href=\"#\" onclick=\"fillSSID(this.innerHTML)\">" + WiFi.SSID(i) + "</a><br>";
      delay(10);
    }
  }
  return wifiHtmlList;
}

//Web Pages Functions
String pageContent = "";
String styleContent = "";
String scriptContent = "";

String getPage()
{
  String htmlBody = "< !DOCTYPE html >";
  htmlBody += "<head>";
  htmlBody += styleContent;
  htmlBody += "<style>";
  htmlBody += "</style>";
  htmlBody += "</ head>";
  htmlBody += "< body >";
  htmlBody += pageContent;
  htmlBody += "<script>";
  htmlBody += scriptContent;
  htmlBody += "</ script>";
  htmlBody += "</ body> ";
  return htmlBody;
}
void addPageContent(String contentPart)
{
  pageContent += contentPart;
}
void addPageStyle(String stylePart)
{
  styleContent += stylePart;
}
void addPageScript(String scriptPart)
{
  scriptContent += scriptPart;
}
void serveConfigPage()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.softAPdisconnect(true);

  String styles = "";
  styles += "html {display: table;margin: auto;font-family: \"Metropolis\", sans-serif;}";
  styles += "body {display: table-cell;vertical-align: middle;background: #182239;color: #d4def7;}";
  styles += "input {width: 100%;box-sizing: border-box;line-height: 1.5;background: #121a2b;border-radius: 3px;border: 0px solid transparent;color: #d4def7;padding: 0.5em 0.8em;height: 2.5rem;line-height: 1.5;background: #121a2b;width: 100%;display: block;}";
  styles += "a {display: block;color: #DDE7F5;text-decoration:underline;}";
  addPageStyle(styles);

  String scripts = "";
  scripts += "function fillSSID(value) {\r\n";
  scripts += "document.getElementById(\"wifi-ssid\").value = value;\r\n";
  scripts += "}";
  addPageScript(scripts);

  String body = "";
  body += "<h2>WIFI Configuration</h2>";
  body += "<a href='#'>Refresh</a>";
  body += "<div class=\"wifi-list\">";
  body += wifiScan();
  body += "</div>";
  body += "<form method='get' action=''><div class='wifi-form'>";
  body += "<label>SSID: </label><input name='wifi-ssid' id='wifi-ssid' length=32 type='text'><br>";
  body += "<label>Heslo: </label><input name='wifi-pasw' length=32 type='password'><br>";
  body += "<label>Api token: </label><input name='apiToken' length=32 type='password'><br>";
  body += "<input type='submit' value='Connect'>";
  body += "</div></form>";
  addPageContent(body);

  //Routing
  server.on("/", []() {
    if (server.args() == 3)
    {
      ssid = server.arg("wifi-ssid");
      pasw = server.arg("wifi-pasw");
      apiToken = server.arg("apiToken");
      if (ssid != "" && pasw != "" && apiToken != "")
      {
        CleanEeprom();
        WriteEeprom(ssid);
        WriteEeprom(pasw, 33);
        WriteEeprom(apiToken, 65);
        server.send(200, "application/json", "Restarting esp");
        delay(500);
        ESP.restart();
      }
    }
    server.send(200, "text/html", getPage());
  });

  server.onNotFound([]() {
    if (server.args() == 3)
    {
      ssid = server.arg("wifi-ssid");
      pasw = server.arg("wifi-pasw");
      apiToken = server.arg("apiToken");
      if (ssid != "" && pasw != "" && apiToken != "")
      {
        CleanEeprom();
        WriteEeprom(ssid);
        WriteEeprom(pasw, 33);
        WriteEeprom(apiToken, 65);
        server.send(200, "application/json", "Restarting esp");
        delay(500);
        ESP.restart();
      }
    }
    server.send(200, "text/html", getPage());
  });

  //Captive Portal
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
}

//Root/Core Functions
void ICACHE_RAM_ATTR handleInterruptFalling()
{
  buttonPushed = true;
  SetRelayState(!state);
}
void ICACHE_RAM_ATTR handleInterruptRising()
{
  buttonPushed = false;
}

void setup()
{
  EEPROM.begin(100);
  Serial.begin(115200);
  while (!Serial)
    continue;
  delay(2000);
  Serial.println("Booted-UP");

  //debug
  CleanEeprom();
  WriteEeprom("*");
  WriteEeprom("*", 33);
  WriteEeprom("*", 65);

  //read saved data
  ssid = ReadEeprom(1, 33);
  pasw = ReadEeprom(33, 65);
  apiToken = ReadEeprom(65, 97);

  //set pins
  pinMode(SONOFF_LED, OUTPUT);
  pinMode(SONOFF_BUT, INPUT);
  pinMode(SONOFF_RELAY, OUTPUT);

  attachInterrupt(SONOFF_BUT, handleInterruptFalling, FALLING);
  //attachInterrupt(SONOFF_BUT, handleInterruptRising, HIGH);

  //Load Last known State
  SetRelayLastState();

  //Wifi Conection
  if (!wifiConnect(ssid, pasw, true))
  {
    serveConfigPage();
    return;
  }

  //Check OTA Updates
  otaHandler();

  //Diag Data sendData
  StaticJsonDocument<250> jsonContent = {};
  jsonContent["token"] = apiToken;
  jsonContent["values"]["on/off"]["value"] = (int)state;
  jsonContent["settings"]["network"]["ip"] = WiFi.localIP().toString();
  jsonContent["settings"]["network"]["mac"] = WiFi.macAddress();
  jsonContent["settings"]["firmware_hash"] = ESP.getSketchMD5();
  sendData(jsonContent);
}
void loop()
{

  if (!waitForWifi(1))
  {
    serveConfigPage();
    dnsServer.processNextRequest();
    server.handleClient();
    waity++;
    if (waity > 500000)
    {
      waity = 0;
      wifiConnect(ssid, pasw);
    }
    delay(1);
    return;
  }

  StaticJsonDocument<250> jsonContent = {};
  jsonContent["token"] = apiToken;

  if (buttonPushed)
  {
    jsonContent["values"]["wifi"]["value"] = (long)WiFi.RSSI();
    jsonContent["values"]["wifi"]["unit"] = "dBm";
    jsonContent["values"]["on/off"]["value"] = state;
    jsonContent["values"]["wifi"]["unit"] = "";
    Serial.println("Sending State to server - " + String(state));
    buttonPushed = false;
  }

  if (!sendData(jsonContent))
  {
    Serial.println("REQ Failed");
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

  String hostname = jsonObject["device"]["hostname"];
  bool serverState = jsonObject["values"]["on/off"];
  if (serverState != state)
  {
    if (buttonPushed)
    {
      return;
    }
    SetRelayState(serverState);
  }

  if (jsonObject.containsKey("command"))
  {
    commandExecution(jsonObject["command"]);
  }

  WiFi.hostname(hostname);
}