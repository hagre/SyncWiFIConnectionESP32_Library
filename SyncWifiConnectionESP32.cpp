/*
Arduino Library for very simple and basic Wifi connection, SYNCHRONOUS, arduino-esp32

require #include <WiFi.h>

require #include <verysimpletimer.h> https://github.com/hagre/VerySimpleTimer_Library.git

int8_t LANStatus = -5; //Connected to Network //WIFI or ETHERNET //-5 = init, -2 = just disconnected, -1 = wait to reconnect, 0 = disconnected, 1 = connecting, 2 = just connected,  3 = still connected

GPLv3
by hagre 2020
*/

#include "SyncWifiConnectionESP32.h"

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


SyncWifiConnectionESP32::SyncWifiConnectionESP32 (){
    _wiFiStatus = -5;
    WIFIWaitForConnectionTimer.setIntervalMs (WIFI_WAIT_FOR_CONNECTION); 
    WIFIWaitForReconnectingTimer.setIntervalMs (WIFI_WAIT_FOR_RECONNECTION);
    #ifdef DEBUG_WIFI_ENABLED
        Serial.begin (DEBUG_WIFI_BOUD); //Debug output, usually USB Port;
    #endif
}

int8_t SyncWifiConnectionESP32::Loop (uint32_t millistime){

    int8_t WIFIStatus = WiFi.status();

    if (_wiFiStatus == -5){ //First Loop after boot
        #ifdef DEBUG_WIFI_ENABLED
            Serial.println (" First loop after boot");
        #endif
        WIFIWaitForConnectionTimer.resetTimingNow (millistime);
        _wiFiStatus = 0; //disconnected
    }
    else if (_wiFiStatus == -2){ //First Loop after WIFI just disconnected
        WIFIWaitForReconnectingTimer.resetTimingNow (millistime);
        _wiFiStatus = -1; //wait to reconnect
        #ifdef DEBUG_WIFI_ENABLED
            Serial.println (" First Loop after WIFI just disconnected");
        #endif
    }
    else if (_wiFiStatus == -1){ //wait to reconnect still disconnected
        if (WIFIWaitForReconnectingTimer.getStatus(millistime) >= 0){ //Chech if ready for reconnect
            _wiFiStatus = 0;  //disconnected
            #ifdef DEBUG_WIFI_ENABLED
                Serial.println (" Timer elapsed - time to connect again");
            #endif
        }
    }
    else if (_wiFiStatus == 0){ //disconnected and time to connect again
        WiFi.begin(); //WiFi.begin(YOUR_WIFI_SSID, YOUR_WIFI_PASSWORD);
        WIFIWaitForConnectionTimer.resetTimingNow (millistime); //Start new timing for NEW connection
        _wiFiStatus = 1; //connecting
        #ifdef DEBUG_WIFI_ENABLED
            Serial.println (" Starting WIFI connection");
        #endif
    }
    else if (_wiFiStatus == 1){ //connecting
        if (WIFIStatus == WL_CONNECTED){ //WIFI connected
            _wiFiStatus = 2; //connected to LAN
            #ifdef DEBUG_WIFI_ENABLED
                Serial.println (" Just WIFI Connected");
            #endif  
        }
        else if (WIFIWaitForConnectionTimer.getStatus(millistime) >= 0){ //Chech if try to connect takes too long
            WiFi.disconnect();
            _wiFiStatus = -2; //just disconnected
            #ifdef DEBUG_WIFI_ENABLED
                Serial.println (" It took to long => WIFI Disconnect");
            #endif
        }
        else {
            #ifdef DEBUG_WIFI_ENABLED
                //Serial.print (".");
            #endif
        }
    }
    else if (_wiFiStatus == 2){ //just connected
        _wiFiStatus = 3; //still connected to LAN
        #ifdef DEBUG_WIFI_ENABLED
            Serial.println (" Again, this important info: WIFI connected");
        #endif 
    }
    else if (_wiFiStatus == 3){ //still connected
        #ifdef DEBUG_WIFI_ENABLED
            //Serial.println ("WIFI still connected");
        #endif
        if (WIFIStatus != WL_CONNECTED){ //WIFI NOT connected anymore
            WiFi.disconnect();
            _wiFiStatus = -2; //just disconnected
            #ifdef DEBUG_WIFI_ENABLED
                Serial.println (" WIFI connection lost!");
            #endif
        }
    }

    return _wiFiStatus;
}