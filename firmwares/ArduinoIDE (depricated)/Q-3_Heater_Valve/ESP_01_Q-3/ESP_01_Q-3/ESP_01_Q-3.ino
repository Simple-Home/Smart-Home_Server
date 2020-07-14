//Includes
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "ESP8266httpUpdate.h"

//Variables
const char* ssidServer = "";
const char* paswServer = "";
String ssid = "";
String pasw = "";
String apiToken = "";
const int httpsPort = 443;
const char* host = "http://dev.steelants.cz";
const char* url = "/vasek/home/api.php";

const char* fingerprint = "";
const char* host2 = "dev.steelants.cz";
const char* url2 = "/vasek/home/update.php";

String content;
String Logs;
bool conf = false;
bool buttonActive = false;
float lastTemperature = 0;
String requestJson = "";
int unsuccessfulRounds = 0; //Unsucesful atmpt counter
int waity = 0;

const byte DNS_PORT = 53;
ESP8266WebServer server(80);
StaticJsonDocument<265> jsonContent;
DeserializationError error;
DNSServer dnsServer;

//Pins
#define OUT0 0
#define OUT1 2

void setup() {
  Serial.begin(9600);
  EEPROM.begin(100);
  while (!Serial) continue;
  delay(10);
  
  //read saved data
  ssid = ReadEeprom(1, 33);
  pasw = ReadEeprom(33, 65);
  apiToken = ReadEeprom(65, 97);

  //set pins
  pinMode(OUT0, OUTPUT);
  pinMode(OUT1, OUTPUT);
  //wifi
  if (ssid != "") {
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);
    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pasw);
    conf = wifiVerify(20);
    if (conf) {
      configTime(3 * 3600, 0, "pool.ntp.org");
      WiFiClientSecure client;
      Serial.print("connecting to ");
      Serial.println(host2);
      client.setInsecure();
      if (!client.connect(host2, httpsPort)) {
        addLog("connection failed");
        Serial.println("connection failed");
        return;
      }
    
      if (fingerprint == "" || client.verify(fingerprint, host2)) {
        Serial.println("certificate matches");
      } else {
        addLog("certificate doesn't match");
        Serial.println("certificate doesn't match");
        return;
      }

      Serial.print("Starting OTA from: ");
      Serial.println(url2
      
      );
  
      auto ret = ESPhttpUpdate.update(client, host2, 80, url2);
      delay(500);
    switch(ret) {
        case HTTP_UPDATE_FAILED:
            addLog("HTTP_UPDATE_FAILD Error (" + (String)ESPhttpUpdate.getLastError() + ") : " + (String)ESPhttpUpdate.getLastErrorString().c_str());
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            Serial.println();
            Serial.println();
            Serial.println();
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            Serial.println();
            Serial.println();
            break;

        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            Serial.println();
            Serial.println();
            Serial.println();
            break;
    }
    delay(500);
      jsonContent = {};
      jsonContent["token"] = apiToken;
      jsonContent["settings"]["network"]["ip"] = WiFi.localIP().toString();
      jsonContent["settings"]["network"]["mac"] = WiFi.macAddress();
      jsonContent["settings"]["firmware_hash"] = ESP.getSketchMD5();
      sendDataToWeb();
      return;
    }
  }
  setupAP();
}

void loop() {
  if (conf) {
    LogErrors();
    if (unsuccessfulRounds >= 5) {
      Serial.println("RESTARTING ESP");
      unsuccessfulRounds = 0;
      ESP.restart();
    }
    jsonContent = {};
    jsonContent["token"] = apiToken;
    jsonContent["values"]["wifi"]["value"] = (long)WiFi.RSSI();
    jsonContent["values"]["wifi"]["unit"] = "dBm";
    requestJson = "";

    sendDataToWeb();
    loadDataFromWeb();
  } else {
    //redirect after connect to ap
    dnsServer.processNextRequest();
    server.handleClient();
    waity++;
    if (waity > 500000) {
      waity = 0;
      ESP.reset();
    }
    delay(1);
  }
}

bool wifiVerify(int t) {
  int c = 0;
  Serial.println("Waiting for Wifi");
  while (c < t) {
    if (WiFi.status() == WL_CONNECTED) {
      c = t;
      return true;
    }
    delay(500);
    Serial.print(WiFi.status());
    c++;
  }
  return false;
}

void loadDataFromWeb() {
  if (error.code() != DeserializationError::Ok) {
    addLog(error.c_str());
    Serial.println(error.c_str());
    unsuccessfulRounds++;
    Serial.println("UNSUCCESSFUL ROUND NUMBER " + String(unsuccessfulRounds) + "FROM 5");
    error = DeserializationError::Ok;
    return;
  }

  //configuration setup
  int sleepTime = jsonContent["device"]["sleepTime"];
  String hostName = jsonContent["device"]["hostname"];
  String requestState = jsonContent["state"];
  String command = jsonContent["command"];
  float temperature = (int)jsonContent["value"];

  //setTemperature
  if (lastTemperature != temperature) {
    setTemperature(temperature);
    lastTemperature = temperature;
  }
  
  if (command == "reset"){
    ESP.reset();
  } else if (command == "config") {
    CleanEeprom();
    EEPROM.commit();
    ESP.restart();
  }

  if (requestState != "succes") {
    unsuccessfulRounds++;
    Serial.println("UNSUCCESSFUL ROUND NUMBER " + String(unsuccessfulRounds) + "FROM 5");
  } else if (requestState == "succes") {
    unsuccessfulRounds = 0;
  }

  WiFi.hostname(hostName);
  sleep(sleepTime);
}

void sendDataToWeb() {
  serializeJson(jsonContent, requestJson);
  Serial.println("JSON: " + requestJson);
  error = deserializeJson(jsonContent, sendHttpRequest());
}

void sleep(int sleepTime) {
  if (sleepTime > 0) { //if deep sleepTime > 0 use deep sleep
      Serial.println("GOING TO SLEEP FOR " + String(sleepTime));
      ESP.deepSleep((sleepTime * 60) * 1000000, RF_DEFAULT);
  } else {
      delay(5000);
  }
}

String sendHttpRequest () {
  HTTPClient http;
  http.setReuse(true);
  Serial.println("HTTP url: " + String(host) + String(url) + ""); //Print HTTP return code
  http.begin(String(host) + String(url));
  http.addHeader("Content-Type", "text/plain");  //Specify content-type header
  Serial.println("HTTP request: " + String(requestJson) + ""); //Print HTTP return code
  int httpCode = http.POST(requestJson);
  String payload = http.getString();  //Get the response payload
  http.end();

  Serial.println("HTTP CODE: " + String(httpCode) + ""); //Print HTTP return code
  Serial.println("HTTP BODY: " + String(payload) + "");  //Print request response payload

  if (httpCode == -1) {
    unsuccessfulRounds++;
    Serial.println("UNSUCCESSFUL ROUND NUMBER " + String(unsuccessfulRounds) + "FROM 5");
    return "";
  }
  return payload;
}

void LogErrors() {
  if (Logs != ""){
    jsonContent = {};
    error = deserializeJson(jsonContent, "{\"logs\":[" + Logs + "]}");
    jsonContent["token"] = apiToken;
    requestJson = "";
    Logs = "";
    sendDataToWeb();  
    loadDataFromWeb();
  }  
}

void addLog(String logText) {
  if (Logs == "") {
    Logs = "\"" + logText + "\"";
  } else {
    Logs += ",\"" + logText + "\"";
  }
}

void CleanEeprom() {
  for (int i = 1; i < 100; ++i) {
    EEPROM.write(i, 0);
  }
}

void WriteEeprom (String data, int start = 1) {
  for (int i = 0; i < data.length(); ++i)
  {
    EEPROM.write(start + i, data[i]);
  }
  EEPROM.commit();
}

String ReadEeprom(int min, int max) {
  String localString;
  for (int i = min; i < max; ++i) {
    localString += char(EEPROM.read(i));
  }
  return localString;
}

void createWeb()
{
  content = "<!DOCTYPE HTML><body>";
  content += "<head><style>";
  content += "html,body {height: 100%;}";
  content += "html {display: table;margin: auto;font-family: \"Metropolis\", sans-serif;}";
  content += "body {display: table-cell;vertical-align: middle;background: #182239;color: #d4def7;}";
  content += "input {width: 100%;box-sizing: border-box;line-height: 1.5;background: #121a2b;border-radius: 3px;border: 0px solid transparent;color: #d4def7;padding: 0.5em 0.8em;height: 2.5rem;line-height: 1.5;background: #121a2b;width: 100%;display: block;}";
  content += "a {display: block;color: #DDE7F5;text-decoration:underline;}";
  content += "</style></head>";
  content += "<h2>WIFI Configuration</h2>";
  content += "<a href='#'>Refresh</a>";
  content += "<div class=\"wifi-list\">";
  int n = WiFi.scanNetworks();
  if (n == 0)
    content += "<label>No networks found...</label>";
  else
  {
    for (int i = 0; i < n; ++i)
    {
      content += "<a href=\"#\" onclick=\"fillSSID(this.innerHTML)\">" + WiFi.SSID(i) + "</a><br>";
    }
  }
  content += "</div>";
  content += "<form method='get' action=''><div class='wifi-form'>";
  content += "<label>SSID: </label><input name='wifi-ssid' id='wifi-ssid' length=32 type='text'><br>";
  content += "<label>Heslo: </label><input name='wifi-pasw' length=32 type='password'><br>";
  content += "<label>Api token: </label><input name='apiToken' length=32 type='password'><br>";
  content += "<input type='submit' value='Connect'>";
  content += "</div></form>";
  content += "<script>";
  content += "function fillSSID(value) {\r\n";
  content += "document.getElementById(\"wifi-ssid\").value = value;\r\n";
  content += "}";
  content += "</script>";
  content += "</body>";
  
  server.on("/", []() {
    if (server.args() == 3) {
      ssid = server.arg("wifi-ssid");
      pasw = server.arg("wifi-pasw");
      apiToken = server.arg("apiToken");
      if (ssid != "" && pasw != "" && apiToken != "") {
        CleanEeprom();
        WriteEeprom(ssid);
        WriteEeprom(pasw, 33);
        WriteEeprom(apiToken, 65);
        server.send(200, "application/json", "Restarting esp");
        delay(500);
        ESP.restart();
      }
    }
    server.send(200, "text/html", content);
  });

  server.onNotFound([]() {
    if (server.args() == 3) {
      ssid = server.arg("wifi-ssid");
      pasw = server.arg("wifi-pasw");
      apiToken = server.arg("apiToken");
      if (ssid != "" && pasw != "" && apiToken != "") {
        CleanEeprom();
        WriteEeprom(ssid);
        WriteEeprom(pasw, 33);
        WriteEeprom(apiToken, 65);
        server.send(200, "application/json", "Restarting esp");
        delay(500);
        ESP.restart();
      }
    }
    server.send(200, "text/html", content);
  });
}

void setupAP(void) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
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
      delay(10);
    }
  }
  delay(100);
  WiFi.softAP(ssidServer, paswServer);
  Serial.println("softap");
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWeb();

  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  
  // Start the server
  server.begin();
  Serial.println("Server started");
  Serial.println("over");
}

void setTemperature(float temp) {
  int z = 0;
  // Set temperature to OFF
  for(uint8_t i = 0; i < 60; ++i) {
    digitalWrite(OUT1, z);
    delay(30);
    digitalWrite(OUT0, z);
    delay(30);

    z = (z+1) % 2;
  }

  /* Substract 4.5 from the given temperature to compute the number of steps
   * required to set the desired temperature, as the temperature directly jumps
   * from OFF tp 5C. */
  for(uint8_t i = 0; i < (temp-4.5)*2; ++i) {
    digitalWrite(OUT0, z);
    delay(30);
    digitalWrite(OUT1, z);
    delay(30);

    z = (z+1) % 2;
  }
}
