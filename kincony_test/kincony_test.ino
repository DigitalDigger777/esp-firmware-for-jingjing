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
//bool newDevice = true;
int delayInterval = 3000;

int pin = 12; //pin for Node MCU

void setup() {
  //init serial
  Serial.begin(9600);

  WiFiManager wifiManager;
  wifiManager.resetSettings();
  
  wifiManager.autoConnect();

  pinMode(pin, OUTPUT);
  
}

void loop() {
  int httpCode = 0;
  
  if((WiFiMulti.run() == WL_CONNECTED)) {
    Serial.println("...start...");
    digitalWrite(pin, LOW);
    delay(delayInterval);
    digitalWrite(pin, HIGH);
  }
  delay(delayInterval);
}
