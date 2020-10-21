#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(9600);
  for (uint8_t t = 4; t > 0; t--) {
    Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SSID", "PASSSWORD");
}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
      String url = getUrl("1");
    if (http.begin(client, url)) {
      int httpCode = http.GET(); 
      http.end();
    } 
  }
  else{
    getUrl("0");
  }
  delay(100);
}

String getUrl(String id){
  DynamicJsonDocument doc(1024);
  doc["type"] = "request";
  doc["id"] = id;
  serializeJson(doc,Serial);
  boolean messageReady = false;
  String message = "";
  while(messageReady == false) { 
    if(Serial.available()) {
      message = Serial.readString();
      messageReady = true;
    }
  }
  DeserializationError error = deserializeJson(doc,message);
  if(error) {

  }
  return doc["distance"].as<String>();
}
