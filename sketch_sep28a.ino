#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

StaticJsonDocument<128> doc;
const char* ssid = "NETIASPOT-98C2";
const char* password = "7g4D6UcG6qp6";
const char* serverName1 = "http://mts.wibro.agh.edu.pl/~s403062/projekt_inzynierski/esp-post-data.php";
const char* serverName2 = "http://mts.wibro.agh.edu.pl/~s403062/projekt_inzynierski/JSONtoReturn.php";
String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "DHT11";
String sensorLocation = "Home";
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

void setup() {
  Serial.begin(9600);
WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
 while(Serial.available()>0){
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
 http.begin(client, serverName1);
// Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
     String httpRequestData = Serial.readString();
     //Serial.print(httpRequestData);
      int httpResponseCode = http.POST(httpRequestData);
//JSON 


http.end();      


http.begin(client, serverName2);

httpResponseCode=http.GET();
String payload = http.getString();

DeserializationError error = deserializeJson(doc, payload);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}

const char* TurnOnAlarm = doc["TurnOnAlarm"]; // "yes"
const char* TurnOnHeating = doc["TurnOnHeating"]; // "yes"
const char* SetTemperature = doc["SetTemperature"]; // "21"
Serial.print(TurnOnAlarm);

http.end();
}
}
}
