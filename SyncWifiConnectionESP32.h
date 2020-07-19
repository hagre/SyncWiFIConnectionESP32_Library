/*
Arduino Library for very simple and basic Wifi connection, SYNCHRONOUS, arduino-esp32

require #include <WiFi.h>

require #include <verysimpletimer.h>  https://github.com/hagre/VerySimpleTimer_Library.git

int8_t LANStatus = -5; //Connected to Network //WIFI or ETHERNET //-5 = init, -2 = just disconnected, -1 = wait to reconnect, 0 = disconnected, 1 = connecting, 2 = just connected,  3 = still connected

GPLv3
by hagre 2020
*/

#ifndef syncwificonnectionesp32_h
#define syncwificonnectionesp32_h

#include <Arduino.h>
#include <WiFi.h>
#include <verysimpletimer.h> 

#ifndef WIFI_WAIT_FOR_CONNECTION
    #define WIFI_WAIT_FOR_CONNECTION 10000 //ms time
#endif
#ifndef WIFI_WAIT_FOR_RECONNECTION
    #define WIFI_WAIT_FOR_RECONNECTION 5000 //ms time
#endif

//switch ASAP off
//#define DEBUG_WIFI_ENABLED 

#ifdef DEBUG_WIFI_ENABLED
    #define DEBUG_WIFI_BOUD 230400 //config as required
#endif

class SyncWifiConnectionESP32
{
public:
    SyncWifiConnectionESP32();
    void InitAndBegin (wifi_mode_t m, IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1, const char * hostname, const char* ssid, const char *passphrase);
    int8_t Loop (uint32_t millistime);

 
private:
    int8_t _wiFiStatus;

    const char* _ssid;
    const char* _passphrase;

    VerySimpleTimer _WIFIWaitForConnectionTimer;
    VerySimpleTimer _WIFIWaitForReconnectingTimer;
};

#endif