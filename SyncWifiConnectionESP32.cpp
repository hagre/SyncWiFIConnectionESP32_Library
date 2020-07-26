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


SyncWifiConnectionESP32::SyncWifiConnectionESP32 (){
    _wiFiStatus = -5;
    _WIFIWaitForConnectionTimer.setIntervalMs (WIFI_WAIT_FOR_CONNECTION); 
    _WIFIWaitForReconnectingTimer.setIntervalMs (WIFI_WAIT_FOR_RECONNECTION);
    _reconnectCounter = 0;
}

void SyncWifiConnectionESP32::setWifiDebugSerial (HardwareSerial* wifiDebugSerial){
    _wifiDebugSerial = wifiDebugSerial;
}

void SyncWifiConnectionESP32::init (wifi_mode_t m, IPAddress local_ip, const char * hostname, const char* ssid, const char *passphrase){
    _ssid = ssid;
    _passphrase = passphrase;
    _hostname = hostname;
    _localIP = local_ip; 
    _m = m;
    _reconnectCounter = 0;
}

int8_t SyncWifiConnectionESP32::loop (uint32_t millistime){

    int8_t WIFIStatus = WiFi.status();

    if (_wiFiStatus == -5){ //First Loop after boot

        _reconnectCounter = 0;

        #ifdef DEBUG_MY_WIFI_ENABLED
            _wifiDebugSerial->println (" First loop after boot or WIFI total reset");
        #endif

        WiFi.mode(WIFI_OFF);
        WiFi.setHostname(_hostname);
        WiFi.begin(_ssid, _passphrase);

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            #ifdef DEBUG_MY_WIFI_ENABLED
                _wifiDebugSerial->println (":");
            #endif
        }

        #ifdef DEBUG_MY_WIFI_ENABLED
            _wifiDebugSerial->println("WIFI dhcp configured");
            _wifiDebugSerial->println(WiFi.localIP());
            _wifiDebugSerial->println(WiFi.getHostname());
            _wifiDebugSerial->println(WiFi.getAutoConnect());
            _wifiDebugSerial->println(WiFi.getAutoReconnect());
            _wifiDebugSerial->println(WiFi.gatewayIP());
            _wifiDebugSerial->println(WiFi.dnsIP());
            _wifiDebugSerial->println(WiFi.broadcastIP());
            _wifiDebugSerial->println(WiFi.status());
        #endif 

        _gatewayIP = WiFi.gatewayIP();
        _subnetIP = WiFi.subnetMask(); 
        _dnsIP = WiFi.dnsIP();
        WiFi.mode(WIFI_OFF); 
        WiFi.mode(_m);
        WiFi.setHostname(_hostname);
        WiFi.config(_localIP, _gatewayIP, _subnetIP, _dnsIP);// secDNS);
        WiFi.begin(_ssid, _passphrase);

        #ifdef DEBUG_MY_WIFI_ENABLED
            _wifiDebugSerial->println("WIFI manual configured");
            _wifiDebugSerial->println(WiFi.localIP());
            _wifiDebugSerial->println(WiFi.getHostname());
            _wifiDebugSerial->println(WiFi.getAutoConnect());
            _wifiDebugSerial->println(WiFi.getAutoReconnect());
            _wifiDebugSerial->println(WiFi.gatewayIP());
            _wifiDebugSerial->println(WiFi.dnsIP());
            _wifiDebugSerial->println(WiFi.broadcastIP());
            _wifiDebugSerial->println(WiFi.status());
        #endif 

        _WIFIWaitForConnectionTimer.resetTimingNow (millistime);
        _wiFiStatus = 0; //disconnected
    }
    else if (_wiFiStatus == -2){ //First Loop after WIFI just disconnected
        _WIFIWaitForReconnectingTimer.resetTimingNow (millistime);
        _wiFiStatus = -1; //wait to reconnect
        #ifdef DEBUG_MY_WIFI_ENABLED
            _wifiDebugSerial->println (" First Loop after WIFI just disconnected");
        #endif
    }
    else if (_wiFiStatus == -1){ //wait to reconnect still disconnected
        if (_reconnectCounter > MAX_RECONNECT_COUNTER){
            _wiFiStatus = -5;  //reset like first Boot
            #ifdef DEBUG_MY_WIFI_ENABLED
                _wifiDebugSerial->println (" now it is time to reset the wifi completely");
            #endif
        }
        if (_WIFIWaitForReconnectingTimer.getStatus(millistime) >= 0){ //Chech if ready for reconnect
            _reconnectCounter++;
            _wiFiStatus = 0;  //disconnected
            #ifdef DEBUG_MY_WIFI_ENABLED
                _wifiDebugSerial->println (" Timer elapsed - time to connect again");
            #endif
        }
    }
    else if (_wiFiStatus == 0){ //disconnected and time to connect again
        WiFi.begin(_ssid, _passphrase);
        _WIFIWaitForConnectionTimer.resetTimingNow (millistime); //Start new timing for NEW connection
        _wiFiStatus = 1; //connecting
        #ifdef DEBUG_MY_WIFI_ENABLED
            _wifiDebugSerial->println (" Starting WIFI connection");
        #endif
    }
    else if (_wiFiStatus == 1){ //connecting
        if (WIFIStatus == WL_CONNECTED){ //WIFI connected
            _wiFiStatus = 2; //connected to LAN
            #ifdef DEBUG_MY_WIFI_ENABLED
                _wifiDebugSerial->println (" Just WIFI Connected");
            #endif  
        }
        else if (_WIFIWaitForConnectionTimer.getStatus(millistime) >= 0){ //Chech if try to connect takes too long
            WiFi.disconnect();
            _wiFiStatus = -2; //just disconnected
            #ifdef DEBUG_MY_WIFI_ENABLED
                _wifiDebugSerial->println (" It took to long => WIFI Disconnect");
            #endif
        }
        else {
            #ifdef DEBUG_MY_WIFI_ENABLED
                _wifiDebugSerial->print (".");
            #endif
        }
    }
    else if (_wiFiStatus == 2){ //just connected
        _wiFiStatus = 3; //still connected to LAN
        #ifdef DEBUG_MY_WIFI_ENABLED
            _wifiDebugSerial->println (" Again, this important info: WIFI connected");
        #endif 
    }
    else if (_wiFiStatus == 3){ //still connected
        #ifdef DEBUG_MY_WIFI_ENABLED
            _wifiDebugSerial->println ("WIFI still connected");
        #endif
        if (WIFIStatus != WL_CONNECTED){ //WIFI NOT connected anymore
            WiFi.disconnect();
            _wiFiStatus = -2; //just disconnected
            #ifdef DEBUG_MY_WIFI_ENABLED
                _wifiDebugSerial->println (" WIFI connection lost!");
            #endif
        }
    }

    return _wiFiStatus;
}