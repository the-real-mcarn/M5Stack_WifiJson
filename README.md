## M5Stack_WifiJson Library (Work in progress)
### Easy networking for M5Stack
A library that reads WiFi credentials from your SD card and connects to a the nearest known network

#### Requirements
- M5Stack that works with Arduino IDE
- An SD card formatted to work with your M5Stack
- M5Stack Libraries - <https://docs.m5stack.com/>
- ArduinoJson Libraries - <https://arduinojson.org/>
- esp_wpa2.h (You have this if you have esptool)
- A json file anywhere on the SD card with the following formatting:

`[
    ["ssid", "password"],
    ["ssid", "password"],
    ["ssid", "password", "username"], // WPA Enterprise
    ...
]`

#### Known issues
- The json file has a file size limit of 500 bytes, the amount of networks you will be able to store entirely depends on the length of the passwords. 
- Sometimes the library returns true without being connected which can cause a problem if your program requires an internet connection, usually just resetting your Arduino solves
- The library will not check if the network has internet access