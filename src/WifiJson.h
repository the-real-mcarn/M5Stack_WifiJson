#ifndef WifiJson_h
#define WifiJson_s
 
#include <Arduino.h>
#include <M5Stack.h>
#include <ArduinoJson.h>

class WifiJson
{
public:
    WifiJson(bool setSilent = false);
    void readJson(const char* filename);
private:
    bool silent;
    File jsonSource;
    String jsonString;
    StaticJsonDocument<500> json;
};

#endif