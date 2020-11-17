//CONFIGURATION FILE OF Simple-Home Unified Firmware

/*
    Settings Type settings
*/
//#define MOMENTARY_SWITCH
#define ON_OFF_SWITCH

#define ENABLE_OTA
//#define ENABLE_SERIAL_PRINT
#define ENABLE_SERVER_LOGS
#define USE_EPRROM_WIFI_SETING
#define WIFI_CONFIG_PAGE

//pins

#ifdef WIFI_CONFIG_PAGE
    #define CONFIG_AP_SSID "FILIP_ROOM_WALL_SWITCH_0"
    #define CONFIG_AP_PASSWOR ""
#endif

#ifndef USE_EPRROM_WIFI_SETING
    #define WIFI_SSID "Smart-Home"
    #define WIFI_PASSWORD ""
    #define API_TOKEN ""
#endif

