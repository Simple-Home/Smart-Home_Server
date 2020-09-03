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

#ifdef WIFI_CONFIG_PAGE
    #define CONFIG_AP_SSID "Filip_ROOM_WALL_SWITCH_0"
    #define CONFIG_AP_PASSWOR "55f4g8d6ggh"
#else
    #define WIFI_SSID "FILL"
    #define WIFI_PASSWORD "FILL"
    #define API_TOKEN "FILL"
#endif

