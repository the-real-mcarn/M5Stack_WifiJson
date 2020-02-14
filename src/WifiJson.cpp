#include <Arduino.h>
#include <M5Stack.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "WifiJson.h"
#include "esp_wpa2.h"

WifiJson::WifiJson(bool setSilent)
{
    M5.begin();
    silent = setSilent;

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
            M5.Lcd.print(jsonSource.name());
            M5.Lcd.print("  Size: ");
            M5.Lcd.println(jsonSource.size());
        }

        Serial.print("File: ");
        Serial.print(jsonSource.name());
        Serial.print("  Size: ");
        Serial.println(jsonSource.size());

        while (jsonSource.available())
        {
            char output = jsonSource.read();
            jsonString += output;
        }
        deserializeJson(json, jsonString);
        jsonSource.close();

        wifiArray = json.as<JsonArray>();

        if (!silent)
        {
            M5.Lcd.println("Known networks:");
            for (JsonVariant v : wifiArray)
            {
                M5.Lcd.print(v[0].as<String>().c_str());
                M5.Lcd.print(" ");
            }
            M5.Lcd.println("\n");
        }

        return true;
    }
    else
    {
        if (!silent)
        {
            M5.Lcd.print(filename);
            M5.Lcd.println(" does not exist.");
        }
        Serial.print(filename);
        Serial.println(" does not exist.");

        return false;
    }
}

bool WifiJson::matchSSID()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

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
            for (JsonVariant v : wifiArray)
            {
                Serial.print("   Match " + v[0].as<String>());                
                if (WiFi.SSID(i) == v[0])
                {
                    known = true;
                    Serial.println(" - True");

                    if (v[2])
                    {
                        // Enterprise Network
                        Serial.print(" (Enterprise Network)");
                        Serial.println("Using credentials:  " + v[2].as<String>() + " : " + v[1].as<String>());
                        
                        if ( connect( v[0].as<String>().c_str(), v[1].as<String>().c_str(), v[2].as<String>().c_str() ) )
                        {
                            return true;
                        }
                    } else {
                        // Personal Network
                        Serial.print(" (Personal Network)");
                        Serial.println("Using credentials:  " + v[1].as<String>());

                        if ( connect( v[0].as<String>().c_str(), v[1].as<String>().c_str() ) )
                        {
                            return true;
                        }
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

bool WifiJson::connect(const char *ssid, const char *password, const char *username)
{
    WiFi.disconnect();
    timeout = 10;
    delay(100);

    if (!silent)
    {
        M5.Lcd.print("Connecting to ");
        M5.Lcd.print(ssid);
    }

    if (username == "")
    {
        // WPA Personal
        if (!silent)
        {
            M5.Lcd.print(" (WPA Personal)");
        }

        WiFi.begin(ssid, password);
    } else {
        // WPA Enterprise
        if (!silent)
        {
            M5.Lcd.print(" (WPA Enterprise)");
        }

        esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
        esp_wifi_sta_wpa2_ent_set_username((uint8_t *)username, strlen(username));
        esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password, strlen(password));
        esp_wifi_sta_wpa2_ent_enable(&config);
        WiFi.begin(ssid);
    }
    

    

    while (WiFi.status() != WL_CONNECTED)
    {
        if (WiFi.status() == WL_CONNECT_FAILED || timeout == 0)
        {
            if (!silent)
            {
                M5.Lcd.println("\nConnection failure, check password and DHCP settings");
            }
            Serial.println("\nConnection failure, check password and DHCP settings");
            return false;
        }
        else
        {
            if (!silent)
            {
                M5.Lcd.print(".");
            }

            timeout = timeout - 1;
            delay(1000);
        }
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        ip = WiFi.localIP().toString();
        if (!silent)
        {
            M5.Lcd.println(" OK");
        }
        Serial.println("Connection succesfull.");
        return true;
    }
}
