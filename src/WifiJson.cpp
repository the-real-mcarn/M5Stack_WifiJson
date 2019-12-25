#include <Arduino.h>
#include <M5Stack.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "WifiJson.h"

WifiJson::WifiJson(bool setSilent)
{
    // Initialise M5Stack if it hasn't been done yet
    M5.begin();

    // Set global variables
    silent = setSilent;

    // Free advertising
    if (!silent)
    {
        M5.Lcd.println("M5Stack_WifiJson by McArn \n");
    }
    Serial.println("M5Stack_WifiJson by McArn \n");
}

bool WifiJson::begin(const char *filename)
{
    if (initSD())
    {
        if (readJson(filename))
        {
            if (matchSSID())
            {
                if (!silent)
                {
                    M5.Lcd.println("Connection established!");
                }
                Serial.println("Connection established!");
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool WifiJson::initSD()
{
    // Initialise SD card
    if (!silent)
    {
        M5.Lcd.print("SD Initialising... ");
    }
    Serial.print("SD Initialising... ");
    if (!SD.begin(4))
    {
        if (!silent)
        {
            M5.Lcd.println("Failed!");
        }
        Serial.println("Failed!");
        return false;
    }
    if (!silent)
    {
        M5.Lcd.println("OK");
    }
    Serial.println("OK");
    return true;
}

bool WifiJson::readJson(const char *filename)
{
    if (SD.exists(filename))
    {
        jsonSource = SD.open(filename);

        if (!silent)
        {
            M5.Lcd.print("File: ");
        }
        Serial.print("File: ");
        if (!silent)
        {
            M5.Lcd.print(jsonSource.name());
        }
        Serial.print(jsonSource.name());

        if (!silent)
        {
            M5.Lcd.print("  Size: ");
        }
        Serial.print("  Size: ");
        if (!silent)
        {
            M5.Lcd.println(jsonSource.size());
        }
        Serial.println(jsonSource.size());

        while (jsonSource.available())
        {
            char output = jsonSource.read();
            jsonString += output;
        }
        deserializeJson(json, jsonString);
        jsonSource.close();

        return true;
    }
    else
    {
        if (!silent)
        {
            M5.Lcd.print(filename);
        }
        Serial.print(filename);
        if (!silent)
        {
            M5.Lcd.println(" does not exist.");
        }
        Serial.println(" does not exist.");

        return false;
    }
}

bool WifiJson::matchSSID()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    JsonArray wifiArray = json.as<JsonArray>();

    // Start scan
    if (!silent)
    {
        M5.Lcd.print("WiFi scan started...");
    }
    Serial.print("WiFi scan started...");
    int n = WiFi.scanNetworks();

    if (!silent)
    {
        M5.Lcd.println("OK");
    }
    Serial.println("OK");

    if (n > 0)
    {
        for (size_t i = 0; i < n; i++)
        {
            Serial.println("Found " + WiFi.SSID(i));
            // Match SSID to those in wifi.json
            for (JsonVariant v : wifiArray)
            {
                Serial.print("   Match " + v[0].as<String>());
                if (WiFi.SSID(i) == v[0])
                {
                    // Set known to true
                    known = true;

                    Serial.println(" - True");
                    Serial.println(v[1].as<String>().c_str());

                    if (connect(v[0].as<String>().c_str(), v[1].as<String>().c_str()))
                    {
                        ip = WiFi.localIP();
                        return true;
                    }
                }
                else
                {
                    Serial.println(" - False");
                }
            }
        }
        if (!known)
        {
            if (!silent)
            {
                M5.Lcd.println("No known networks found!");
            }
            Serial.println("No known networks found!");
            return false;
        }
    }
    else
    {
        if (!silent)
        {
            M5.Lcd.println("No networks found!");
        }
        Serial.println("No networks found!");
        return false;
    }
}

bool WifiJson::connect(const char* ssid[], const char* password[])
{
    if (!silent)
    {
        M5.Lcd.print("Connecting to ");
        M5.Lcd.print(ssid);
    }
    for (int attempts = 0; attempts < 10; ++attempts)
    {
        status = WiFi.begin(ssid, password);
        Serial.println("Connection attempt: " + attempts);

        if (status != WL_CONNECTED)
        {
            if (status == WL_CONNECT_FAILED)
            {
                // If so break loop and notify user
                Serial.println("Connection failure, check password and DHCP settings");
                M5.Lcd.println("Connection failure, check password and DHCP settings");
                return false;
            }
            else
            {
                // If not, wait a second and try again
                M5.Lcd.print(".");
                delay(1000);
            }
        }
        else
        {
            // Return true if so
            Serial.println("Connection succesfull.");
            M5.Lcd.println(" OK");
            return true;
        }
    }
    return false;
}