//CONFIGURATION FILE OF Simple-Home Unified Firmware

//#define MOMENTARY_SWITCH
#define ON_OFF_SWITCH
#define WIFI_CONFIG_PAGE
#define ENABLE_OTA
#define ENABLE_SERIAL_PRINT
#define ENABLE_SERVER_LOGS
#define USE_EPRROM_WIFI_SETING

#ifdef WIFI_CONFIG_PAGE
    #define CONFIG_AP_SSID "FILL"
    #define CONFIG_AP_PASSWOR "FILL"
#else
    #define WIFI_PASSWORD "FILL"
    #define WIFI_SSID "FILL"
    #define API_TOKEN "FILL"
#endif

