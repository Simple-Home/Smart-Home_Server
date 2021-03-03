#include "HttpManager.h"


HttpManager::HttpManager(char* host, char* port, char* url, char* token){
    this->host = host;
    this->port = port;
    this->url = url;
    this->token = token;
}

void HttpManager::connect()
{
    //retrun true false
    this->https.begin(this->client, String(this->host) + String(this->url));
    this->https.setReuse(true);
    this->https.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    this->https.setRedirectLimit(1);
    this->https..addHeader("Authorization","Bearer " + String(this->token));
    this->https.addHeader("Content-Type", "application/json");

    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("HttpManager-ConecteTo" + String(this->host + this->url));
        Serial.println("HttpManager-Token" + String(this->token));
    #endif
}

bool HttpManager::send(char* requiresBody)
{   
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("HttpManager->" + requiresBody);
    #endif
    int httpCode = this->https.POST(requiresBody);
    if (httpCode != 200){
        return false
    }

    this->payload = this->https.getString();
    #ifdef ENABLE_SERIAL_PRINT
        Serial.print("HttpManager<-" + (String) httpsCode);
        Serial.print("HttpManager<-" + (String) this->payload);
    #endif
 
    return true;
}

char* HttpManager::getPayload()
{   
    return this->payload;
}

bool HttpManager::disconect()
{
    this->https.end();
}
    