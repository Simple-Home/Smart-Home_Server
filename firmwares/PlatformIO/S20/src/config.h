//CONFIGURATION FILE OF Simple-Home Unified Firmware

//Features Nice WIFI Configuration

//#define WIFI_CONFIG_PAGE
//#define ENABLE_OTA
//#define ENABLE_SERIAL_PRINT
//#define ENABLE_SERVER_LOGS







#ifdef WIFI_CONFIG_PAGE
    #define WIFI_PASSWORD
    #define WIFI_SSID
    #define API_TOKEN
#else
    #define CONFIG_AP_SSID
    #define CONFIG_AP_PASSWOR
#endif

