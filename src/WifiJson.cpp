#include <Arduino.h>
#include <M5Stack.h>
#include <ArduinoJson.h>
#include "WifiJson.h"

WifiJson::WifiJson(bool setSilent)
{
    // Initialise M5Stack if it hasn't been done yet
    M5.begin();

    // Set global variables
    silent = setSilent;

    // Free advertising
    if (!silent) { M5.Lcd.println("M5Stack_WifiJson by McArn \n"); }
    Serial.println("M5Stack_WifiJson by McArn \n");
}

bool WifiJson::begin(const char* filename) {
    if (initSD())
    {
        if (readJson(filename))
        {
            if (!silent) { M5.Lcd.println("Connection established!"); }
            Serial.println("Connection established!");
            return true;
        } else { return false; }
    } else { return false; }
    
}

bool WifiJson::initSD() {
    // Initialise SD card
    if (!silent) { M5.Lcd.print("SD Initialising... "); }
    Serial.print("SD Initialising... ");
    if (!SD.begin(4))
    {
        if (!silent) { M5.Lcd.println("Failed!"); }
        Serial.println("Failed!");
        return false;
    }
    if (!silent) { M5.Lcd.println("OK"); }
    Serial.println("OK");
    return true;
}

bool WifiJson::readJson(const char *filename)
{
    if (SD.exists(filename))
    {
        jsonSource = SD.open(filename);

        if (!silent) { M5.Lcd.print("File: "); }
        Serial.print("File: ");
        if (!silent) { M5.Lcd.print(jsonSource.name()); }
        Serial.print(jsonSource.name());

        if (!silent) { M5.Lcd.print("  Size: "); }
        Serial.print("  Size: ");
        if (!silent) { M5.Lcd.println(jsonSource.size()); }
        Serial.println(jsonSource.size());

        while (jsonSource.available())
        {
            char output = jsonSource.read();
            jsonString += output;
        }
        deserializeJson(json, jsonString);
        jsonSource.close();

        return true;
    } else {
        if (!silent) { M5.Lcd.print(filename); }
        Serial.print(filename);
        if (!silent) { M5.Lcd.println(" does not exist."); }
        Serial.println(" does not exist.");

        return false;
    }
}

bool WifiJson::matchSSID() {
    return true;
}

bool WifiJson::connect() {
    return true;
}
