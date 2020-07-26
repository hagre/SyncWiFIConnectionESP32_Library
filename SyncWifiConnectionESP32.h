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
#ifndef MAX_RECONNECT_COUNTER
    #define MAX_RECONNECT_COUNTER 10 //times
#endif

//switch ASAP off
//#define DEBUG_MY_WIFI_ENABLED 

#include <HardwareSerial.h>


class SyncWifiConnectionESP32
{
public:
    SyncWifiConnectionESP32();
    
    void init (wifi_mode_t m, IPAddress local_ip, const char * hostname, const char* ssid, const char *passphrase);
    int8_t loop (uint32_t millistime);

    void setWifiDebugSerial (HardwareSerial* wifiDebugSerial);


 
private:

    HardwareSerial* _wifiDebugSerial;

    int8_t _wiFiStatus;
    uint16_t _reconnectCounter;

    IPAddress _localIP; 
    IPAddress _gatewayIP; 
    IPAddress _subnetIP; 
    IPAddress _dnsIP;

    const char* _ssid;
    const char* _passphrase;
    const char* _hostname;

    wifi_mode_t _m;

    VerySimpleTimer _WIFIWaitForConnectionTimer;
    VerySimpleTimer _WIFIWaitForReconnectingTimer;
};

#endif