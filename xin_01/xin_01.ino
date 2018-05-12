#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h> 
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

ESP8266WiFiMulti WiFiMulti;

char http_server[40];

bool shouldSaveConfig = false;
int delayInterval = 3000;

//GPION12 => D6
int gpio12 = 12;

void setup() {
  Serial.begin(9600);

  WiFiManager wifiManager;

  //reset saved settings need for debug for production uncomment for debug
  //wifiManager.resetSettings();


  wifiManager.autoConnect();
  pinMode(gpio12, OUTPUT);

  
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
      String addDeviceQuery = "http://xin.jjpanda.com/public/index.php/install/device?mac=";
      
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

void updateFirmware(String mac){
    
    if ((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;
        t_httpUpdate_return ret = ESPhttpUpdate.update("http://xin.jjpanda.com/public/xin_01.ino.bin");
        String changeStatusQuery = "http://xin.jjpanda.com/public/index.php/update-firmware/change-status?mac=";
        
        switch (ret) {
            case HTTP_UPDATE_FAILED:
                  Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                  //http://xin.jjpanda.com/public/index.php/update-firmware/change-status?mac=mac&status=0
                  http.begin(changeStatusQuery + mac + "&status=0");
                  http.GET();
              break;
      
            case HTTP_UPDATE_NO_UPDATES:
                  Serial.println("HTTP_UPDATE_NO_UPDATES\n");
                  //http://xin.jjpanda.com/public/index.php/update-firmware/change-status?mac=mac&status=1
                  http.begin(changeStatusQuery + mac + "&status=1");
                  http.GET();
              break;
      
            case HTTP_UPDATE_OK:
                  Serial.println("HTTP_UPDATE_OK");
                  //http://xin.jjpanda.com/public/index.php/update-firmware/change-status?mac=mac&status=2
                  http.begin(changeStatusQuery + mac + "&status=2");
                  http.GET();
              break;
        }
    }
      
}

void loop() {
  int httpCode = 0;
  
  if((WiFiMulti.run() == WL_CONNECTED)) {
  
    Serial.println('.');
    HTTPClient http;
    
    Serial.print("[HTTP] begin...\n");
    String mac =  WiFi.macAddress();

    //check status
    String checkStatusQuery = "http://xin.jjpanda.com/public/index.php/check-interval?mac=";
    
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

      //update firmware
      if (httpCode == 206) {
        updateFirmware(mac);
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
  }
  
  delay(delayInterval);
}
