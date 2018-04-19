#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h> 
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;

char http_server[40];

bool shouldSaveConfig = false;
int delayInterval = 3000;

//GPION12 => D6
int gpio12 = 12;
int gpioReset = 0;

void setup() {
  Serial.begin(9600);

  WiFiManager wifiManager;

  //reset saved settings need for debug for production uncomment for debug
  wifiManager.resetSettings();


  wifiManager.autoConnect();
  pinMode(gpio12, OUTPUT);
  pinMode(gpioReset, INPUT_PULLUP);
  
}

void loop() {

}
