#include "config.h"
#include "WebPageManager.h"

WebPageManager::WebPageManager(char* configApName, char* configApPassword)
{
    this->configApName = configApName;
    this->configApPassword = configApPassword;
}

void WebPageManager::StartPage()
{
    if (configPage) {
        configPage = false;
        this->ConfigPage();
    }
    this->dnsServer.processNextRequest();
    this->server.handleClient();
}

void WebPageManager::ConfigPage()
{
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Serving Config Page");
    #endif
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);
    WiFi.softAP(this->configApName, this->configApPassword);

    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Wifi - Soft AP");
      Serial.print("Local IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("SoftAP IP: ");
      Serial.println(WiFi.softAPIP());
    #endif

    char* styles = "";
    styles += F("html {display: table;margin: auto;font-family: \"Metropolis\", sans-serif;}");
    styles += F("body {display: table-cell;vertical-align: middle;background: #182239;color: #d4def7;}");
    styles += F("input {width: 100%;box-sizing: border-box;line-height: 1.5;background: #121a2b;border-radius: 3px;border: 0px solid transparent;color: #d4def7;padding: 0.5em 0.8em;height: 2.5rem;line-height: 1.5;background: #121a2b;width: 100%;display: block;}");
    styles += F("a {display: block;color: #DDE7F5;text-decoration:underline;}");
    styles += F("input[type=submit] {display: block;color: white; background-color: #265cd9;}");
    addPageStyle(styles);

    char* scripts = "";
    scripts += F("function fillSSID(value) {\r\n");
    scripts += F("document.getElementById(\"wifi-ssid\").value = value;\r\n");
    scripts += F("}");
    scripts += F("document.getElementById(\"api-token\").value = Math.random().toString(36).replace(/[^a-z0-9]+/g, '').substr(1, 12);\r\n");
    addPageScript(scripts);

    //Routing
    server.on("/", serverResponseHandler);
    server.onNotFound(serverResponseHandler);
    server.begin();
    
    //Captive Portal
    dnsServer.start(this->DNS_PORT, "*", WiFi.softAPIP());
}
