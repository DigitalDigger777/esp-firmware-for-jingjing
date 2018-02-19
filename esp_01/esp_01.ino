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
bool newDevice = true;
int delayInterval = 3000;

byte relON[] = {0xA0, 0x01, 0x01, 0xA2};  //Hex command to send to serial for open relay
byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1}; //Hex command to send to serial for close relay

void setup() {
  //init serial
  Serial.begin(9600);

  WiFiManager wifiManager;
  //reset saved settings need for debug for production need comment
  //wifiManager.resetSettings();
  newDevice = true;

  
  //WiFiManagerParameter custom_http_server("server", "http server", "jingjing.com", 40);
  //wifiManager.addParameter(&custom_http_server);
  
  wifiManager.autoConnect();

  //strcpy(http_server, "http://");
  //strcat(http_server, custom_http_server.getValue());
  
}

void loop() {
  int httpCode = 0;
  if((WiFiMulti.run() == WL_CONNECTED)) {
    Serial.println('.');
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    String mac =  WiFi.macAddress();
    //check new device
    if (newDevice) {
      String addDeviceQuery = "http://jingjing.fenglinfl.com/public/index.php/install/device?mac=";
      
      
      Serial.println(addDeviceQuery + mac);
      
      http.begin(addDeviceQuery + mac);
      httpCode = http.GET();

      if (httpCode > 0) {
        Serial.println("Saved device");
      } else {
          Serial.println("Error");
          Serial.print(httpCode);
      }
      newDevice = false;
    }

    //check status
    String checkStatusQuery = "http://jingjing.fenglinfl.com/public/index.php/check-interval?mac=";
    
    http.begin(checkStatusQuery + mac);
    Serial.println(checkStatusQuery + mac);
    
    httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println(payload);
      
      if (httpCode == 204) {
          delayInterval = 3000;
          Serial.write(relOFF, sizeof(relOFF));
      }
      
      if (httpCode == 200) {
          delayInterval = 60*1000;
          Serial.write(relON, sizeof(relON));
      }
      
      if (httpCode == 500) {
          delayInterval = 5000;
          Serial.write(relOFF, sizeof(relON));
      }      
    } else {
      delayInterval = 5000;
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());  
    }
  }
  delay(delayInterval);
}
