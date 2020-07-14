//Includes
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

//Variables
const char* ssid   = "Smart-Home";
const char* pasw   = "S1pjg3684dcCPTUQ";
const char* hwId   = "45235sadad";
const char* url    = "http://dev.steelants.cz/vasek/home/api.php";
int unsuccessfulRounds = 0;

//Pins
#define pinDHT 4
#define LIGHTPIN 13

//Inicializations
DHT DHTs(pinDHT, DHT11);

void setup() {
    Serial.begin(9600);
    while (!Serial) continue;
    delay(10);
    Serial.println('\n');
    //Show start up Configuration
    Serial.println("HW: " + String(hwId));
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
    Serial.print("MAC address:\t");
    Serial.println(WiFi.macAddress());
    pinMode(LIGHTPIN, INPUT);
}

void loop() {
    //Start Conection to wifi
    WiFi.begin(ssid, pasw);
    checkConnection();

    //HTTP CLIENT
    HTTPClient http;
    http.begin(url); //Begun HTTP Request
    http.addHeader("Content-Type", "text/plain");  //Specify content-type header

    DHTs.begin();

    //JsonDocsDefinition
    StaticJsonDocument<265> doc;
    doc["token"] = hwId;

    //Read and Handle DHT values
    float tem = DHTs.readTemperature();
    float hum = DHTs.readHumidity();
    Serial.println("TEMP" + String(tem) + ";HUMI" + String(hum));
    if (isnan(tem) || isnan(hum)) {
        Serial.println("Unable to read DHT");
    } else {
        doc["values"]["temp"]["value"] = tem;
        doc["values"]["temp"]["unit"] = "C";
        doc["values"]["humi"]["value"] = hum;
        doc["values"]["humi"]["unit"] = "%";
    }

    //Handle Photo Rezistor Values
    doc["values"]["light"]["value"] = (String)!digitalRead(LIGHTPIN);
    doc["values"]["light"]["unit"] = "";

    /*More Senzores to come*/
    String jsonPayload = "";
    serializeJson(doc, jsonPayload);
    Serial.print("JSON: ");
    Serial.println(jsonPayload);

    int httpCode = http.POST(jsonPayload); //Get Http response code
    String httpPayload = http.getString();  //Get the response payload
    Serial.println("HTTP CODE: " + String(httpCode) + ""); //Print HTTP return code
    Serial.println("HTTP BODY: " + String(httpPayload) + "");  //Print request response payload

    DeserializationError error = deserializeJson(doc, httpPayload); //Get deserialization Error if exists

    //configuration setup
    int sleepTime = doc["device"]["sleepTime"];
    String hostName = doc["device"]["hostname"];
    String ipAddress = doc["device"]["ipAddress"];
    String gateway = doc["device"]["gateway"];
    String subnet = doc["device"]["subnet"];
    String state = doc["state"];

    if (state != "succes") {
        unsuccessfulRounds++;
        Serial.println("UNSUCCESSFUL ROUND NUMBER " + (String)unsuccessfulRounds + "FROM 5");
    } else if (state == "succes") {
        unsuccessfulRounds = 0;
    }

    //Set static ip
    setStaticIp(ipAddress, gateway, subnet);
    WiFi.hostname(hostName); //Set HostName

    http.end();  //Close connection
    WiFi.disconnect(); //Disconect from WIFI
    Serial.println("DISCONECTED FROM WIFI");

    if(unsuccessfulRounds == 5) { //after 5 unsucessful request restart ESP
        Serial.println("RESTARTING ESP");
        ESP.restart();
    }

    sleep(sleepTime);
}

//checking if connection is working
bool checkConnection() {
    int count = 0;
    Serial.print("Waiting for Wi-Fi connection");
    while ( count < 30 ) {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println();
            Serial.println("Connected!");
            return (true);
        }
        delay(500);
        Serial.print(".");
        count++;
    }
    Serial.println("Timed out.");
    return false;
}
void setStaticIp(String IpAddress, String subnet, String gateway){
  //Set static ip
  IPAddress staticIpAddress;
  IPAddress subnetIpAddress;
  IPAddress gatewayIpAddress;

  if (
    staticIpAddress.fromString(IpAddress) &&
    subnetIpAddress.fromString(subnet) &&
    gatewayIpAddress.fromString(gateway) &&
    WiFi.localIP() != staticIpAddress
  ) {
      WiFi.config(staticIpAddress, subnetIpAddress, gatewayIpAddress);
      Serial.print("STATIC IP address:\t");
      Serial.println(WiFi.localIP());
  }
}
void sleep(int sleepTime) {
  if (sleepTime > 0) { //if deep sleepTime > 0 use deep sleep
      Serial.println("GOING TO SLEEP FOR " + String(sleepTime));
      ESP.deepSleep((sleepTime * 60) * 1000000, RF_DEFAULT);
  } else {
      delay(5000);
  }
}
