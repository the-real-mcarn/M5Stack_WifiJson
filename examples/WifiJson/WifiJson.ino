#include <M5Stack.h>
#include <WifiJson.h>

// New instance of WifiJson, silent is false, it will log to the M5Stacks screen
WifiJson wifi(false);

void setup() {
  M5.begin();

  /* 
    Attempt connecting to a network using credentials in /wifi.json.
    Will return false if connection fails
  */
  if (!wifi.begin("/wifi.json"))
  {
    Serial.println("This program requires a WiFi connection!");
    M5.Lcd.println("This program requires a WiFi connection!");  
    
    // Do not let the program run 
    while (true)
    { }
  }
  Serial.println(wifi.ip);
  M5.Lcd.println(wifi.ip);  

  Serial.print("Waiting...");
  M5.Lcd.print("Waiting...");  
}

void loop() {
  Serial.print(".");
  M5.Lcd.print("."); 
  delay(10000);
}
