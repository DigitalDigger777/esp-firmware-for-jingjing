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
  //wifiManager.resetSettings();


  wifiManager.autoConnect();
  pinMode(gpio12, OUTPUT);
  pinMode(gpioReset, INPUT_PULLUP);

  while(!checkDevice()) {
    delay(3000);  
  }
  
}

/**
 * Check device, if device not find our database, then add it. 
 * return boolean, if device find or was add, will be return true, else return false.
 */
bool checkDevice() {
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      
      HTTPClient http;
      int httpCode = 0;
  
      String mac =  WiFi.macAddress();
      String addDeviceQuery = "http://jingjing.fenglinfl.com/public/index.php/install/device?mac=";
      
      http.begin(addDeviceQuery + mac);
      httpCode = http.GET();

      if (httpCode == 201) {
        Serial.println("Saved device");
      }

      if (httpCode == 200) {
        Serial.println("DB have device");
      }

      if (httpCode == 0) {
        Serial.println("Error: http code 0");
      }
      
      return true;
    } else {
      
      return false;
      
    }
}

void loop() {
  int httpCode = 0;

  //reset 
  int reset_level = digitalRead(gpioReset);
  
  Serial.print("reset_level: ");
  Serial.println(reset_level);

  WiFiManager wifiManager;
  
  if (reset_level == 0) {
      
      wifiManager.resetSettings();
      WiFi.disconnect(true);
      delay(2000);
      ESP.reset();
  }
    
  if((WiFiMulti.run() == WL_CONNECTED)) {
  
    Serial.println('.');
    HTTPClient http;
    
    Serial.print("[HTTP] begin...\n");
    String mac =  WiFi.macAddress();

    //check status
    String checkStatusQuery = "http://jingjing.fenglinfl.com/public/index.php/check-interval?mac=";
    
    http.begin(checkStatusQuery + mac);
    Serial.println(checkStatusQuery + mac);

    httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      //if http code 204 then disable relay
      if (httpCode == 204) {
          delayInterval = 3000;
          digitalWrite(gpio12, LOW);
      }

      if (httpCode == 205) {
          //String clearResetQuery = "http://jingjing.fenglinfl.com/public/index.php/clear-reset?mac=";
          //http.begin(clearResetQuery + mac);
          //Serial.println(clearResetQuery + mac);

          //delay(3000);
          wifiManager.resetSettings();
          WiFi.disconnect(true);
          delay(2000);
          ESP.reset();
      }
      
      //if http code 200 then enable relay
      if (httpCode == 200) {
          String json = http.getString();
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& object = jsonBuffer.parseObject(json);
          String intervalStr = object["interval"];
          int interval = intervalStr.toInt();
          
          delayInterval = interval*1000;
          
          digitalWrite(gpio12, HIGH);
          
          Serial.println("Active");
      }

      //if http code 500 then disable relay
      if (httpCode == 500) {
          delayInterval = 5000;
          digitalWrite(gpio12, LOW);
      }
      
    } else {
      
      delayInterval = 5000;
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      
    }
  } else {
      wifiManager.autoConnect();  
  }
  
  delay(delayInterval);
}
