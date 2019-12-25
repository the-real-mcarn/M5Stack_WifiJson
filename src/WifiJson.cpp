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
    if (!silent) { M5.Lcd.println("M5Stack_WifiJson by McArn"); }
    Serial.println("M5Stack_WifiJson by McArn");

    // Initialise SD card
    if (!silent) { M5.Lcd.print("SD Initialising... "); }
    Serial.print("SD Initialising... ");
    if (!SD.begin(4))
    {
        if (!silent) { M5.Lcd.println("Failed!"); }
        Serial.println("Failed!");
        // Do not continue if SD cannot be initialised
        while (true)
        {
        }
    }
    if (!silent) { M5.Lcd.println("OK"); }
    Serial.println("OK");
}

void WifiJson::readJson(const char *filename)
{
    Serial.println(filename);
    jsonSource = SD.open(filename);

    Serial.print("File: ");
    Serial.print(jsonSource.name());
    Serial.print("  Size: ");
    Serial.println(jsonSource.size());

    while (jsonSource.available())
    {
        char output = jsonSource.read();
        jsonString += output;
    }
    Serial.print(jsonString);
    deserializeJson(json, jsonString);
    Serial.print(json.as<String>().c_str());

    jsonSource.close();
}
