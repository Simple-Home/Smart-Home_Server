#include "config.h"
#include "WebPageManager.h"

WebPageManager::WebPageManager(String configApName, String configApPassword, EepromManager eeprom_storage)
{
    this->configApName = configApName;
    this->configApPassword = configApPassword;
    this->eeprom_storage = eeprom_storage;

    this->styleContent = "";
    this->styleContent += F("html {display: table;margin: auto;font-family: \"Metropolis\", sans-serif;}");
    this->styleContent += F("body {display: table-cell;vertical-align: middle;background: #182239;color: #d4def7;}");
    this->styleContent += F("input {width: 100%;box-sizing: border-box;line-height: 1.5;background: #121a2b;border-radius: 3px;border: 0px solid transparent;color: #d4def7;padding: 0.5em 0.8em;height: 2.5rem;line-height: 1.5;background: #121a2b;width: 100%;display: block;}");
    this->styleContent += F("a {display: block;color: #DDE7F5;text-decoration:underline;}");
    this->styleContent += F("input[type=submit] {display: block;color: white; background-color: #265cd9;}");

    this->scriptContent = "";
    this->scriptContent += F("function fillSSID(value) {\r\n");
    this->scriptContent += F("document.getElementById(\"wifi-ssid\").value = value;\r\n");
    this->scriptContent += F("}");
    this->scriptContent += F("document.getElementById(\"api-token\").value = Math.random().toString(36).replace(/[^a-z0-9]+/g, '').substr(1, 12);\r\n");

    this->pageContent = "";
    this->pageContent += F("<h2>WIFI Configuration</h2>");
    this->pageContent += F("<a href='#'>Refresh</a>");
    this->pageContent += F("<div class=\"wifi-list\">");
    this->pageContent += "%wifiScan%";
    this->pageContent += F("</div>");
    this->pageContent += F("<form method='get' action=''><div class='wifi-form'>");
    this->pageContent += F("<label>SSID: </label><input name='wifi-ssid' id='wifi-ssid' length=32 type='text'><br>");
    this->pageContent += F("<label>Heslo: </label><input name='wifi-pasw' length=32 type='password'><br>");
    this->pageContent += F("<label>Api token: </label><input name='apiToken' id='api-token' length=32 type='password'><br>");
    this->pageContent += F("<input type='submit' value='Connect'>");
    this->pageContent += F("</div></form>");

    this->pageHtml = F("<!DOCTYPE html>");
    this->pageHtml += F("<head>");
    this->pageHtml += F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    this->pageHtml += F("<style>");
    this->pageHtml += this->styleContent;
    this->pageHtml += F("</style>");
    this->pageHtml += F("</head>");
    this->pageHtml += F("<body>");
    this->pageHtml += this->pageContent;
    this->pageHtml += F("<script>");
    this->pageHtml += this->scriptContent;
    this->pageHtml += F("</script>");
    this->pageHtml += F("</body>");
}

void WebPageManager::StartPage()
{
  #ifdef ENABLE_SERIAL_PRINT
      Serial.println("WebPage-Running");
  #endif
  if (configPage) {
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("WebPage-Create");
    #endif
    configPage = false;
    this->ConfigPage();
  }
  this->dnsServer.processNextRequest();
  this->server.handleClient();
}

void WebPageManager::ConfigPage()
{
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("WebPage-ServingConfigPage");
    #endif
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);
    WiFi.softAP(this->configApName, this->configApPassword);

    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("WebPage-WifiSoftAP");
      Serial.print("WebPage-LocalIP: ");
      Serial.println(WiFi.localIP());
      Serial.print("WebPage-SoftAPIP: ");
      Serial.println(WiFi.softAPIP());
    #endif

    //Routing
    this->server.on("/", std::bind(&WebPageManager::ResponseHandler, this));
    this->server.onNotFound(std::bind(&WebPageManager::ResponseHandler, this));
    this->server.begin();
    
    //Captive Portal
    dnsServer.start(this->DNS_PORT, "*", WiFi.softAPIP());
}

void WebPageManager::ResponseHandler()
{
    String ssid = "";
    String pasw = "";
    String apiToken = "";
    char ssidCh[33];
    char paswCh[33];
    char apiTokenCh[33];
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("WebPage-RoutingWifiSetting");
    #endif
    if (server.args() == 3)
    {
        if (
            (ssid = server.arg("wifi-ssid")) != "" && 
            (pasw = server.arg("wifi-pasw")) != "" && 
            (apiToken = server.arg("apiToken")) != ""
        ) {
            #ifdef ENABLE_SERIAL_PRINT
                Serial.println(ssid);
                Serial.println(pasw);
                Serial.println(apiToken);
            #endif
            ssid.toCharArray(ssidCh, 33);
            pasw.toCharArray(paswCh, 33);
            apiToken.toCharArray(apiTokenCh, 33);
            this->eeprom_storage.erase(1, 97);
            this->eeprom_storage.write(ssidCh, 1, 33);
            this->eeprom_storage.write(paswCh, 33, 65);
            this->eeprom_storage.write(apiTokenCh, 65, 97);
            this->eeprom_storage.save();
            this->server.send(200, "application/json", "Restarting...");
            delay(500);
            #ifdef ENABLE_SERIAL_PRINT
                Serial.println("WebPage-SavingSetting");
            #endif
            ESP.restart();
        }
    }
    String content = this->pageHtml;
    content.replace(String("%wifiScan%"), String(this->wifiScan()));
    char contentCh[content.length() + 1];
    content.toCharArray(contentCh, content.length() + 1);
    this->server.send(200, "text/html", contentCh);
}

String WebPageManager::wifiScan()
{
  String wifiHtmlList = "";
  int n = WiFi.scanNetworks();
  #ifdef ENABLE_SERIAL_PRINT
    Serial.println("WebPage-WifiScan");
  #endif
  if (n == 0)
  {
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("WebPage-NoNetworksFound");
    #endif
    wifiHtmlList = "<label>No networks found...</label>";
  } else {
    #ifdef ENABLE_SERIAL_PRINT
        Serial.print("WebPage-" + n);
        Serial.println(" networks found");
    #endif
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print("WebPage-" + i + 1);
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

void WebPageManager::Active() {
    this->configPage = true;
}