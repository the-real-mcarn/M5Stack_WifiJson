/*
    WifiJson
        Easy WiFi for M5Stack
        Reads json file from SD card and connects to the nearest known wifi network.

        bool silent:
            Makes the library not print on the M5's lcd
*/

#ifndef WifiJson_h
#define WifiJson_s
 
#include <Arduino.h>
#include <M5Stack.h>
#include <ArduinoJson.h>
#include "WiFi.h"

class WifiJson
{
public:
    WifiJson(bool setSilent = false);
    bool begin(const char* filename);
    IPAddress ip;

private:
    bool silent;
    bool known = false;
    int status = WL_IDLE_STATUS;

    File jsonSource;
    String jsonString;
    StaticJsonDocument<500> json;

    bool initSD();
    bool readJson(const char* filename);
    bool matchSSID();
    bool connect();
};

#endif