//CONFIGURATION FILE OF Simple-Home Unified Firmware

/*
    Settings Type settings
*/
//#define MOMENTARY_SWITCH
#define ON_OFF_SWITCH

#define ENABLE_OTA
#define ENABLE_SERIAL_PRINT
#define ENABLE_SERVER_LOGS
//#define USE_EPRROM_WIFI_SETING
#define WIFI_CONFIG_PAGE

#ifdef WIFI_CONFIG_PAGE
    #define CONFIG_AP_SSID "VASEK_ROOM_WALL_SWITCH_0"
    #define CONFIG_AP_PASSWOR "UeG18ZHtr2"
#endif

#ifndef USE_EPRROM_WIFI_SETING
    #define WIFI_SSID "Smart-Home"
    #define WIFI_PASSWORD "S1pjg3684dcCPTUQ"
    #define API_TOKEN "UeG18ZHtr2"
#endif

