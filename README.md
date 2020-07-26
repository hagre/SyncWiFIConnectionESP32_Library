# SyncWiFIConnectionESP32_Library
Arduino Library for very simple and basic Wifi connection, SYNCHRONOUS, arduino-esp32


require #include <WiFi.h>

require #include <verysimpletimer.h>  https://github.com/hagre/VerySimpleTimer_Library.git

-5 = init, 

-2 = just disconnected, 

-1 = wait to reconnect, 

0 = disconnected, 

1 = connecting, 

2 = just connected,  

3 = still connected



    SyncWifiConnectionESP32();
    
    void InitAndBegin (wifi_mode_t m, IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1, const char * hostname, const char* ssid, const char *passphrase);
    int8_t Loop (uint32_t millistime);

    void setWifiDebugSerial (HardwareSerial* wifiDebugSerial);



GPLv3
by hagre 2020
