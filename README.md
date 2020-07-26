# SyncWiFIConnectionESP32_Library
Arduino Library for very simple and basic Wifi connection, SYNCHRONOUS, arduino-esp32

# Requirements:
require #include <WiFi.h>

require #include <verysimpletimer.h>  https://github.com/hagre/VerySimpleTimer_Library.git

# Status:

-5 = init, or reconnection counter make wifi to reset totaly

-2 = just disconnected, 

-1 = wait to reconnect, 

0 = disconnected, 

1 = connecting, 

2 = just connected,  

3 = still connected


# Settings:
#ifndef WIFI_WAIT_FOR_CONNECTION

    #define WIFI_WAIT_FOR_CONNECTION 10000 //ms time
    
#endif

#ifndef WIFI_WAIT_FOR_RECONNECTION

    #define WIFI_WAIT_FOR_RECONNECTION 5000 //ms time
    
#endif

#ifndef MAX_RECONNECT_COUNTER

    #define MAX_RECONNECT_COUNTER 10 //times
    
#endif

# Public:
    SyncWifiConnectionESP32();
    
    void init (wifi_mode_t m, IPAddress local_ip, const char * hostname, const char* ssid, const char *passphrase);
    int8_t loop (uint32_t millistime);

    void setWifiDebugSerial (HardwareSerial* wifiDebugSerial);



GPLv3
by hagre 2020
