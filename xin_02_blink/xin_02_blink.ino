#include <FS.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

ESP8266WiFiMulti WiFiMulti;
WiFiManager wifiManager;
HTTPClient http;

bool isCheckedDevice = false;

String domain = "http://xin.jjpanda.com/";

int led = 2;
//GPION12 => D6
int gpio12 = 12;

/**
 * Save ssid to file
 */
bool saveSSID() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return false;
  }

  File ssidFile = SPIFFS.open("/ssid.json", "r");
  
  if (!ssidFile) {
    
    ssidFile = SPIFFS.open("/ssid.json", "w");

    //save ssid to file
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["ssid"] = WiFi.SSID();
    json["pass"] = WiFi.psk(); 
    
    Serial.println("Save ssid: ");
    Serial.println(WiFi.SSID());
    
    if (!ssidFile) {
      Serial.println("Failed to open ssid file for writing\n");
      return false;
    }
  
    json.printTo(ssidFile);
  }
  
  return true;
}

/**
 * Read file ssid.json and return ssid
 */
String readSSID() {
  
    if (!SPIFFS.begin()) {
      Serial.println("Failed to mount file system");
    }
    
    File ssidFile = SPIFFS.open("/ssid.json", "r");

    //if file not found then autoConnect
    //this operation start configuration portal if flash not have store ssid and password, 
    //else this operation try connect to wifi
    if (!ssidFile){

      wifiManager.autoConnect();

    }

    //else read file and return ssid
    size_t size = ssidFile.size();
    
    if (size > 1024) {
      Serial.println("Config file size is too large\n");
    }

    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);
  
    // We don't use String here because ArduinoJson library requires the input
    // buffer to be mutable. If you don't use ArduinoJson, you may as well
    // use configFile.readString instead.
    ssidFile.readBytes(buf.get(), size);
  
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(buf.get());
  
    if (!json.success()) {
      Serial.println("Failed to parse ssid file\n");
    }
  
    return json["ssid"];  
}


bool reconnectWiFi() {
    if (!SPIFFS.begin()) {
      Serial.println("*KN: Failed to mount file system");
    }
    
    File ssidFile = SPIFFS.open("/ssid.json", "r");

    //if file not found then autoConnect
    //this operation start configuration portal if flash not have store ssid and password, 
    //else this operation try connect to wifi
    if (!ssidFile){
      Serial.println("*KN: SSID file not found, start configuration portal");
      wifiManager.autoConnect();
    }

    //else read file and return ssid
    size_t size = ssidFile.size();
    
    if (size > 1024) {
      Serial.println("*KN: Config file size is too large\n");
    }

    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);
  
    // We don't use String here because ArduinoJson library requires the input
    // buffer to be mutable. If you don't use ArduinoJson, you may as well
    // use configFile.readString instead.
    ssidFile.readBytes(buf.get(), size);
  
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(buf.get());
  
    if (!json.success()) {
      Serial.println("*KN: Failed to parse ssid file\n");
    }
    String _ssid = json["ssid"];
    String _pass = json["pass"];
    const char *ssid = _ssid.c_str();
    const char *pass = _pass.c_str();

    Serial.println(ssid);
    Serial.println(pass);

    WiFi.begin(ssid, pass);

    Serial.println("*KN: wait connection");
    Serial.println(WiFi.status());
    
    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    Serial.println("*KN: connection established");
    saveSSID();
    return true;
}

/**
 * Reset configuration.
 */
void resetConfiguration() {
    wifiManager.resetSettings();
    WiFi.disconnect(true);
    delay(2000);

    if (SPIFFS.exists("/ssid.json")) {
      SPIFFS.remove("/ssid.json");
    }
    //ESP.reset();
    reconnectWiFi();  
}

/*
  Serial command.
*/
void command() {
  String incomingByte = "";

  if (Serial.available() > 0) {
    incomingByte = Serial.readString();

    Serial.print("I received: ");
    Serial.println(incomingByte);

    //RESET command for reset connection and restart esp8266
    if (incomingByte == "RESET") {
      isCheckedDevice = false;
      resetConfiguration();
    }
  }
}

/**
 * Air update firmware 
 * 
 */
void updateFirmware(String mac){
    
    if ((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;
        t_httpUpdate_return ret = ESPhttpUpdate.update(domain + "public/xin_01.ino.bin");
        String changeStatusQuery = domain + "public/index.php/update-firmware/change-status?mac=";
        
        switch (ret) {
            case HTTP_UPDATE_FAILED:
                  Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                  //public/index.php/update-firmware/change-status?mac=mac&status=0
                  http.begin(changeStatusQuery + mac + "&status=0");
                  http.GET();
              break;
      
            case HTTP_UPDATE_NO_UPDATES:
                  Serial.println("HTTP_UPDATE_NO_UPDATES\n");
                  //public/index.php/update-firmware/change-status?mac=mac&status=1
                  http.begin(changeStatusQuery + mac + "&status=1");
                  http.GET();
              break;
      
            case HTTP_UPDATE_OK:
                  Serial.println("HTTP_UPDATE_OK");
                  //public/index.php/update-firmware/change-status?mac=mac&status=2
                  http.begin(changeStatusQuery + mac + "&status=2");
                  http.GET();
              break;
        }
    }
      
}

/**
 * Server listener.
 */
int listener(){
  String mac =  WiFi.macAddress();  
  String checkStatusQuery = domain + "public/index.php/check-interval?mac=";
  int httpCode;
  int delayInterval = 3000;
  
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

    //reset configuration
    if (httpCode == 205) {
      resetConfiguration();
    }

    //if http code 200 then enable relay
    if (httpCode == 200) {
      String json = http.getString();
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& object = jsonBuffer.parseObject(json);
      String intervalStr = object["interval"];
      int interval = intervalStr.toInt();

      delayInterval = interval * 1000;

      digitalWrite(gpio12, HIGH);

      Serial.println("Active");
    }

    //if http code 500 then disable relay
    if (httpCode == 500) {
      delayInterval = 5000;
      digitalWrite(gpio12, LOW);
    }
    
  } else {
      delayInterval = 3000;  
  }

  return delayInterval;
}

/**
   Check device, if device not find our database, then add it.
   return boolean, if device find or was add, will be return true, else return false.
*/
bool checkDevice() {
    
    int httpCode = 0;

    String mac =  WiFi.macAddress();
    String addDeviceQuery = domain + "public/index.php/install/device?mac=";

    http.begin(addDeviceQuery + mac);
    httpCode = http.GET();

    if (httpCode == 201) {
      Serial.println("Saved device");
    }

    if (httpCode == 200) {
      Serial.println("DB have device");
    }

    if (httpCode <= 0) {
      Serial.println("Error: http code 0");
      isCheckedDevice = false;
    }

    return true;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(led, OUTPUT);
  pinMode(gpio12, OUTPUT);

  delay(500);
  digitalWrite(led, HIGH);
  reconnectWiFi();

  Serial.println("Check device");
  while (!checkDevice()) {
    Serial.print(".");
    delay(3000);
  }
}

void loop() {

  command();

  if (!isCheckedDevice) {
    isCheckedDevice = checkDevice();  
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("*KN: wifi connected");
    
    listener();
    digitalWrite(led, HIGH);
    delay(3000);
    digitalWrite(led, LOW);
  }

  if (WiFi.status() == WL_DISCONNECTED) {
    Serial.println("*KN: wifi disconnected");

    digitalWrite(led, HIGH);
    delay(500);
    WiFi.disconnect(true);
    delay(2000);
    reconnectWiFi();
  }

  if (WiFi.status() == WL_NO_SSID_AVAIL) {
    Serial.println("*KN: no ssid avail");
  }

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("*KN: no shield");
  }

  if (WiFi.status() == WL_CONNECT_FAILED) {
    Serial.println("*KN: connect failed");
  }

  if (WiFi.status() == WL_CONNECTION_LOST) {
    Serial.println("*KN: connect lost");
  }  

  if (WiFi.status() == WL_IDLE_STATUS) {
    Serial.println("*KN: idle status");
  }
  
  Serial.println(WiFi.status());
  delay(3000);
}
