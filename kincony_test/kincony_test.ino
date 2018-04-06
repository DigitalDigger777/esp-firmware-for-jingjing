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

int pin0 = 16; //pin for Node MCU
int pin1 = 5;
int pin2 = 4;
int pin3 = 0;
int pin4 = 2;
int pin5 = 14;
int pin6 = 12;
int pin7 = 13;
int pin8 = 15;
int pin9 = 3;
int pin10 = 1;

void setup() {
  //init serial
  Serial.begin(9600);

  WiFiManager wifiManager;
  wifiManager.resetSettings();
  
  wifiManager.autoConnect();

  pinMode(pin0, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  pinMode(pin8, OUTPUT);
  pinMode(pin9, OUTPUT);
  pinMode(pin10, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  int httpCode = 0;
  
  if((WiFiMulti.run() == WL_CONNECTED)) {
    Serial.println("...start...");
    
    digitalWrite(pin0, HIGH);
    delay(1000);
    digitalWrite(pin0, LOW);

    delay(10000);
    
    digitalWrite(pin1, HIGH);
    delay(1000);
    digitalWrite(pin1, LOW);
    delay(1000);
    
    digitalWrite(pin1, HIGH);
    delay(1000);
    digitalWrite(pin1, LOW);
    delay(1000);
    
    delay(10000);
    
    digitalWrite(pin2, HIGH);
    delay(1000);
    digitalWrite(pin2, LOW);
    delay(1000);
    
    digitalWrite(pin2, HIGH);
    delay(1000);
    digitalWrite(pin2, LOW);
    delay(1000);
    
    digitalWrite(pin2, HIGH);
    delay(1000);
    digitalWrite(pin2, LOW);
    delay(1000);
    
    delay(10000);
    
    digitalWrite(pin3, HIGH);
    delay(1000);
    digitalWrite(pin3, LOW);
    delay(1000);

    digitalWrite(pin3, HIGH);
    delay(1000);
    digitalWrite(pin3, LOW);
    delay(1000);
    
    digitalWrite(pin3, HIGH);
    delay(1000);
    digitalWrite(pin3, LOW);
    delay(1000);
    
    digitalWrite(pin3, HIGH);
    delay(1000);
    digitalWrite(pin3, LOW);
    delay(1000);
    
    delay(10000);

    digitalWrite(pin4, HIGH);
    delay(1000);
    digitalWrite(pin4, LOW);
    delay(1000);

    digitalWrite(pin4, HIGH);
    delay(1000);
    digitalWrite(pin4, LOW);
    delay(1000);

    digitalWrite(pin4, HIGH);
    delay(1000);
    digitalWrite(pin4, LOW);
    delay(1000);
    
    digitalWrite(pin4, HIGH);
    delay(1000);
    digitalWrite(pin4, LOW);
    delay(1000);

    digitalWrite(pin4, HIGH);
    delay(1000);
    digitalWrite(pin4, LOW);
    delay(1000);
    
    delay(10000);
    
    digitalWrite(pin5, HIGH);
    delay(1000);
    digitalWrite(pin5, LOW);
    delay(1000);
    
    digitalWrite(pin5, HIGH);
    delay(1000);
    digitalWrite(pin5, LOW);
    delay(1000);

    digitalWrite(pin5, HIGH);
    delay(1000);
    digitalWrite(pin5, LOW);
    delay(1000);
    
    digitalWrite(pin5, HIGH);
    delay(1000);
    digitalWrite(pin5, LOW);
    delay(1000);

    digitalWrite(pin5, HIGH);
    delay(1000);
    digitalWrite(pin5, LOW);
    delay(1000);

    digitalWrite(pin5, HIGH);
    delay(1000);
    digitalWrite(pin5, LOW);
    delay(1000);

    digitalWrite(pin5, HIGH);
    delay(1000);
    digitalWrite(pin5, LOW);
    delay(1000);
    
    delay(10000);

    digitalWrite(pin6, HIGH);
    delay(1000);
    digitalWrite(pin6, LOW);
    delay(1000);

    digitalWrite(pin6, HIGH);
    delay(1000);
    digitalWrite(pin6, LOW);
    delay(1000);

    digitalWrite(pin6, HIGH);
    delay(1000);
    digitalWrite(pin6, LOW);
    delay(1000);

    digitalWrite(pin6, HIGH);
    delay(1000);
    digitalWrite(pin6, LOW);
    delay(1000);
    
    digitalWrite(pin6, HIGH);
    delay(1000);
    digitalWrite(pin6, LOW);
    delay(1000);

    digitalWrite(pin6, HIGH);
    delay(1000);
    digitalWrite(pin6, LOW);
    delay(1000);

    digitalWrite(pin6, HIGH);
    delay(1000);
    digitalWrite(pin6, LOW);
    delay(1000);
    
    delay(10000);

    digitalWrite(pin7, HIGH);
    delay(1000);
    digitalWrite(pin7, LOW);
    delay(1000);

    digitalWrite(pin7, HIGH);
    delay(1000);
    digitalWrite(pin7, LOW);
    delay(1000);

    digitalWrite(pin7, HIGH);
    delay(1000);
    digitalWrite(pin7, LOW);
    delay(1000);

    digitalWrite(pin7, HIGH);
    delay(1000);
    digitalWrite(pin7, LOW);
    delay(1000);

    digitalWrite(pin7, HIGH);
    delay(1000);
    digitalWrite(pin7, LOW);
    delay(1000);

    digitalWrite(pin7, HIGH);
    delay(1000);
    digitalWrite(pin7, LOW);
    delay(1000);

    digitalWrite(pin7, HIGH);
    delay(1000);
    digitalWrite(pin7, LOW);
    delay(1000);

    digitalWrite(pin7, HIGH);
    delay(1000);
    digitalWrite(pin7, LOW);
    delay(1000);
    
    delay(10000);

    digitalWrite(pin8, HIGH);
    delay(1000);
    digitalWrite(pin8, LOW);
    delay(1000);

    digitalWrite(pin8, HIGH);
    delay(1000);
    digitalWrite(pin8, LOW);
    delay(1000);

    digitalWrite(pin8, HIGH);
    delay(1000);
    digitalWrite(pin8, LOW);

    digitalWrite(pin8, HIGH);
    delay(1000);
    digitalWrite(pin8, LOW);
    delay(1000);

    digitalWrite(pin8, HIGH);
    delay(1000);
    digitalWrite(pin8, LOW);

    digitalWrite(pin8, HIGH);
    delay(1000);
    digitalWrite(pin8, LOW);

    digitalWrite(pin8, HIGH);
    delay(1000);
    digitalWrite(pin8, LOW);
    delay(1000);

    digitalWrite(pin8, HIGH);
    delay(1000);
    digitalWrite(pin8, LOW);
    delay(1000);

    digitalWrite(pin8, HIGH);
    delay(1000);
    digitalWrite(pin8, LOW);
    delay(1000);

    delay(10000);

    digitalWrite(pin9, HIGH);
    delay(1000);
    digitalWrite(pin9, LOW);    
    delay(1000);

    digitalWrite(pin9, HIGH);
    delay(1000);
    digitalWrite(pin9, LOW);    
    delay(1000);
    
    digitalWrite(pin9, HIGH);
    delay(1000);
    digitalWrite(pin9, LOW);    
    delay(1000);

    digitalWrite(pin9, HIGH);
    delay(1000);
    digitalWrite(pin9, LOW);    
    delay(1000);
    
    digitalWrite(pin9, HIGH);
    delay(1000);
    digitalWrite(pin9, LOW);    
    delay(1000);
    
    digitalWrite(pin9, HIGH);
    delay(1000);
    digitalWrite(pin9, LOW);    
    delay(1000);

    digitalWrite(pin9, HIGH);
    delay(1000);
    digitalWrite(pin9, LOW);    
    delay(1000);

    digitalWrite(pin9, HIGH);
    delay(1000);
    digitalWrite(pin9, LOW);    
    delay(1000);
    
    digitalWrite(pin9, HIGH);
    delay(1000);
    digitalWrite(pin9, LOW);    
    delay(1000);

    digitalWrite(pin9, HIGH);
    delay(1000);
    digitalWrite(pin9, LOW);    
    delay(1000);

    digitalWrite(pin9, HIGH);
    delay(1000);
    digitalWrite(pin9, LOW);    
    delay(1000);
    
    delay(10000);

    digitalWrite(pin10, HIGH);
    delay(1000);
    digitalWrite(pin10, LOW);
    delay(1000);

    digitalWrite(pin10, HIGH);
    delay(1000);
    digitalWrite(pin10, LOW);
    delay(1000);

    digitalWrite(pin10, HIGH);
    delay(1000);
    digitalWrite(pin10, LOW);
    delay(1000);

    digitalWrite(pin10, HIGH);
    delay(1000);
    digitalWrite(pin10, LOW);
    delay(1000);

    digitalWrite(pin10, HIGH);
    delay(1000);
    digitalWrite(pin10, LOW);
    delay(1000);

    digitalWrite(pin10, HIGH);
    delay(1000);
    digitalWrite(pin10, LOW);
    delay(1000);

    digitalWrite(pin10, HIGH);
    delay(1000);
    digitalWrite(pin10, LOW);
    delay(1000);

    digitalWrite(pin10, HIGH);
    delay(1000);
    digitalWrite(pin10, LOW);
    delay(1000);

    digitalWrite(pin10, HIGH);
    delay(1000);
    digitalWrite(pin10, LOW);
    delay(1000);

    digitalWrite(pin10, HIGH);
    delay(1000);
    digitalWrite(pin10, LOW);
    delay(1000);

    digitalWrite(pin10, HIGH);
    delay(1000);
    digitalWrite(pin10, LOW);
    delay(1000);
  }

}
