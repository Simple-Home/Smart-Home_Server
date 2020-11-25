//WiFi Functions
bool waitForWifi(int timeout = 30)
{
  int i = 0;
  if (i < timeout) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    #ifdef ENABLE_SERIAL_PRINT
      Serial.println("Waiting for Wifi");
    #endif
  }
  while (i < timeout)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    #ifdef LED_PIN
      ledWaiting();
    #else
      delay(1000);
    #endif
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Connecting.. status: " + String(WiFi.status()));
    #endif
    i++;
  }
  return false;
}
bool wifiConnect(String localSsid, String localPasw, bool waitUntilConnect = false)
{
  WiFi.persistent(false);
  #ifdef ENABLE_SERIAL_PRINT
    Serial.print("SSID:");
    Serial.print(localSsid);
    Serial.println(":");

    Serial.print("Password:");
    Serial.print(localPasw);
    Serial.println(":");
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(localSsid, localPasw);
  if (waitUntilConnect)
  {
    waitForWifi(30);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("Connected!");
    #endif
    return true;
  }
  #ifdef ENABLE_SERIAL_PRINT
    Serial.println("Unable to connect!" + WiFi.status());
  #endif
  return false;
}
String wifiScan()
{
  String wifiHtmlList = "";
  int n = WiFi.scanNetworks();
  #ifdef ENABLE_SERIAL_PRINT
    Serial.println("Wifi scan Done");
  #endif
  if (n == 0)
  {
  #ifdef ENABLE_SERIAL_PRINT
      Serial.println("no networks found");
  #endif
    wifiHtmlList += "<label>No networks found...</label>";
  }
  else
  {
  #ifdef ENABLE_SERIAL_PRINT
      Serial.print(n);
      Serial.println(" networks found");
  #endif
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

#ifdef STATIC_IP_SUPPORT
void setStaticIP(String localSsid, String localPasw, String StaticIp, String GatewayIp, String Subnet){
  //convert ip form string to IP Format
  if (StaticIp != "" && GatewayIp != "" && Subnet != "") {
    IPAddress static_ip;
    static_ip.fromString(StaticIp)
    IPAddress gatevay_ip
    gatevay_ip.fromString(GatewayIp)
    IPAddress subnet
    subnet.fromString(Subnet)

    WiFi.config(StaticIp, GatewayIp, Subnet);
    return wifiConnect(String localSsid, String localPasw, true);
  }
  return false;
}
#endif


#ifdef WIFI_CONFIG_PAGE
//Web Pages Functions
String pageContent = "";
String styleContent = "";
String scriptContent = "";

String getPage()
{
  String htmlBody = F("<!DOCTYPE html>");
  htmlBody += F("<head>");
  htmlBody += styleContent;
  htmlBody += F("<style>");
  htmlBody += F("</style>");
  htmlBody += F("</head>");
  htmlBody += F("<body>");
  htmlBody += pageContent;
  htmlBody += F("<script>");
  htmlBody += scriptContent;
  htmlBody += F("</script>");
  htmlBody += F("</body> )");
  return htmlBody;
}
void serverResponseHandler()
{
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

  //Config Page
  String body = "";
  body += F("<h2>WIFI Configuration</h2>");
  body += F("<a href='#'>Refresh</a>");
  body += F("<div class=\"wifi-list\">");
  body += wifiScan();
  body += F("</div>");
  body += F("<form method='get' action=''><div class='wifi-form'>");
  body += F("<label>SSID: </label><input name='wifi-ssid' id='wifi-ssid' length=32 type='text'><br>");
  body += F("<label>Heslo: </label><input name='wifi-pasw' length=32 type='password'><br>");
  body += F("<label>Api token: </label><input name='apiToken' id='api-token' length=32 type='password'><br>");
  body += F("<input type='submit' value='Connect'>");
  body += F("</div></form>");
  addPageContent(body);

  server.send(200, "text/html", getPage());
}
#ifdef STATIC_IP_SUPPORT
  void serverNetworkSettingResponseHandler()
  {
    if (server.args() == 3)
    {
      //Save Static IP
    }

      //Config Page
      String body = "";
      body += F("<h2>Network Configuration</h2>");
      body += F("<form method='get' action=''><div class='wifi-form'>");
      body += F("<label>Static IP: </label><input name='static-ip' type='text' minlength='7' maxlength='15' size='15' pattern='^((\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$'><br>");
      body += F("<label>Network: </label><input name='static-network' type='text' minlength='7' maxlength='15' size='15' pattern='^((\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$'><br>");
      body += F("<label>gateway: </label><input name='static-gateway' type='text' minlength='7' maxlength='15' size='15' pattern='^((\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$'><br>");
      body += F("<input type='submit' value='Save'>");
      body += F("</div></form>");
      addPageContent(body);

    server.send(200, "text/html", getPage());
  }
#endif
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
  WiFi.softAP(configApName, configApPassword);

  #ifdef ENABLE_SERIAL_PRINT
    Serial.println("Wifi - Soft AP");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
  #endif

  String styles = "";
  styles += F("html {display: table;margin: auto;font-family: \"Metropolis\", sans-serif;}");
  styles += F("body {display: table-cell;vertical-align: middle;background: #182239;color: #d4def7;}");
  styles += F("input {width: 100%;box-sizing: border-box;line-height: 1.5;background: #121a2b;border-radius: 3px;border: 0px solid transparent;color: #d4def7;padding: 0.5em 0.8em;height: 2.5rem;line-height: 1.5;background: #121a2b;width: 100%;display: block;}");
  styles += F("a {display: block;color: #DDE7F5;text-decoration:underline;}");
  addPageStyle(styles);

  String scripts = "";
  scripts += F("function fillSSID(value) {\r\n");
  scripts += F("document.getElementById(\"wifi-ssid\").value = value;\r\n");
  scripts += F("}");
  scripts += F("document.getElementById(\"api-token\").value = Math.random().toString(36).replace(/[^a-z0-9]+/g, '').substr(1, 12);\r\n");
  addPageScript(scripts);



  String body = "";
  body += F("<h2>WIFI Configuration</h2>");
  body += F("<a href='#'>Refresh</a>");
  body += F("<div class=\"wifi-list\">");
  body += wifiScan();
  body += F("</div>");
  body += F("<form method='get' action=''><div class='wifi-form'>");
  body += F("<label>SSID: </label><input name='wifi-ssid' id='wifi-ssid' length=32 type='text'><br>");
  body += F("<label>Heslo: </label><input name='wifi-pasw' length=32 type='password'><br>");
  body += F("<label>Api token: </label><input name='apiToken' id='api-token' length=32 type='password'><br>");
  body += F("<input type='submit' value='Connect'>");
  body += F("</div></form>");
  addPageContent(body);

  //Routing
  server.begin();
  server.on("/", []() {
    serverResponseHandler();
  });
  server.onNotFound([]() {
    serverResponseHandler();
  });

  #ifdef STATIC_IP_SUPPORT
    server.on("/network", []() {
      serverNetworkSettingResponseHandler();
    });
  }

  //Captive Portal
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
}
#endif
