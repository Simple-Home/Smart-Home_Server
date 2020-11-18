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

    +---+-----------------------+
    |     Table Of Content      |
    +---+-----------------------+
    | 1 | Pins                  |
    +---+-----------------------+
    | 2 | Imput/Output Settings |
    +---+-----------------------+
    | 3 | Wifi Settings         |
    +---+-----------------------+
    | 4 | Debug Settings        |
    +---+-----------------------+

    Chode functions Carefully you need more than 50% Flash left for OTA Updates!!!
*/
//#define ENABLE_OTA
#define WIFI_CONFIG_PAGE
#define WIFI_RECONNECT_INTERVAL 500000
//#define USE_EPRROM_WIFI_SETING
//#define WATCHDOG_TRASHOLD 50 // number of error detected befor restart
//#define Deep_Sleep

/*
    Pins Settings
*/
//#define LED_PIN 1
//#define SWITCH1_PIN 1
//#define RELAY1_PIN 1
#define BATT_SENSE A0
#define DHT_PIN 2
#define LIGHT_PIN
//#define MOVE_PIN
//#define PROG_LED_PIN


/*
    Imput/Output Settings
*/
#ifdef SWITCH1_PIN
    #define MOMENTARY_SWITCH
    //#define ON_OFF_SWITCH
    
    #define SWITCH1_DEBOUNCE_INTERVAL 400 //in Miliseconds
    #define RELAY1_RECOVER_STATE_ON_POWER_LOSS //uncoment to enable state recovery after power loss otherwise alwais stai off
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
