## M5Stack_WifiJson Library (Work in progress)
### Easy networking for M5Stack
A library that reads WiFi credentials from your SD card and connects to a the nearest known network

#### Requirements
- M5Stack that works with Arduino IDE
- An SD card formatted to work with your M5Stack
- M5Stack Libraries - <https://docs.m5stack.com/>
- ArduinoJson Libraries - <https://arduinojson.org/>
- A json file anywhere on the SD card with the following formatting:

`[
    ["ssid", "password"],
    ["ssid", "password"]
]`

#### Known issues
- It does not work at all at this point