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

GPLv3
by hagre 2020
