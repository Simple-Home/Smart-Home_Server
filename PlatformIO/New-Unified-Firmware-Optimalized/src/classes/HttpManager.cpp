#include "config.h"
#include "HttpManager.h"


HttpManager::HttpManager(char* host, char* port, char* url, String token){
    this->host = host;
    this->port = port;
    this->url = url;
    this->token = token;
}

bool HttpManager::connect()
{

    //TODO: Implement Https 
    //HINT: https://medium.com/@dfa_31434/doing-ssl-requests-on-esp8266-correctly-c1f60ad46f5e
    this->client.setInsecure();   
    //retrun true false
    this->https.begin(this->client, String(this->host) + String(this->url));
    this->https.setReuse(true);
    this->https.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    this->https.setRedirectLimit(1);
    this->https.addHeader("Authorization","Bearer " + this->token);
    this->https.addHeader("Content-Type", "application/json");

    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("HttpManager-ConecteTo" + String(this->host) + String(this->url));
        Serial.println("HttpManager-Token" + this->token);
    #endif

    return true;
}

bool HttpManager::send(char* requiresBody)
{   
    int httpCode = this->https.POST(requiresBody);
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("HttpManager->" + String(requiresBody));
        Serial.println("HttpManager<-" + String(httpCode));
    #endif

    if (httpCode != 200){
        return false;
    }

    this->payload = this->https.getString();
    #ifdef ENABLE_SERIAL_PRINT
        Serial.println("HttpManager<-" + String(this->payload));
    #endif
 
    return true;
}

String HttpManager::getPayload()
{   
    return this->payload;
}

void HttpManager::disconect()
{
    this->https.end();
}
    