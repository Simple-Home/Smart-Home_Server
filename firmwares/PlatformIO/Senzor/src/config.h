/*
  █████████   ███                            ████                      █████   █████                                  
 ███░░░░░███ ░░░                            ░░███                     ░░███   ░░███                                   
░███    ░░░  ████  █████████████   ████████  ░███   ██████             ░███    ░███   ██████  █████████████    ██████ 
░░█████████ ░░███ ░░███░░███░░███ ░░███░░███ ░███  ███░░███ ██████████ ░███████████  ███░░███░░███░░███░░███  ███░░███
 ░░░░░░░░███ ░███  ░███ ░███ ░███  ░███ ░███ ░███ ░███████ ░░░░░░░░░░  ░███░░░░░███ ░███ ░███ ░███ ░███ ░███ ░███████ 
 ███    ░███ ░███  ░███ ░███ ░███  ░███ ░███ ░███ ░███░░░              ░███    ░███ ░███ ░███ ░███ ░███ ░███ ░███░░░  
░░█████████  █████ █████░███ █████ ░███████  █████░░██████             █████   █████░░██████  █████░███ █████░░██████ 
 ░░░░░░░░░  ░░░░░ ░░░░░ ░░░ ░░░░░  ░███░░░  ░░░░░  ░░░░░░             ░░░░░   ░░░░░  ░░░░░░  ░░░░░ ░░░ ░░░░░  ░░░░░░  
                                   ░███                                                                               
                                   █████                                                                              
                                  ░░░░░                                     Made By JonatanRek & Haitem                      
*/
//CONFIGURATION FILE OF Simple-Home Unified Firmware

#define ENABLE_OTA
#define WIFI_CONFIG_PAGE
#define WIFI_RECONNECT_INTERVAL 500000
//#define USE_EPRROM_WIFI_SETING

/*
    Pins Settings
*/
#define LED_PIN 1
//#define SWITCH1_PIN
//#define RELAY1_PIN
#define DHT_PIN 2
//#define LIGHT_PIN
//#define MOVE_PIN

/*
    Imput/Output Settings
*/
#ifdef SWITCH1_PIN
    #define MOMENTARY_SWITCH
    //#define ON_OFF_SWITCH
#endif
#ifdef DHT_PIN
    //DHT11, DHT22
    #define DHT_TYPLE DHT11
#endif

/*
    Wifi Settings
*/
#ifdef WIFI_CONFIG_PAGE
    #define CONFIG_AP_SSID ""
    #define CONFIG_AP_PASSWOR ""
#endif

#ifndef USE_EPRROM_WIFI_SETING
    //oweride setting in EEPROM
    #define WIFI_SSID ""
    #define WIFI_PASSWORD ""
    #define API_TOKEN ""
#endif

/*
    Debug Settings
*/
#define ENABLE_SERIAL_PRINT
#define ENABLE_SERVER_LOGS
