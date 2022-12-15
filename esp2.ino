#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define AlarmTurnOnPin D0
#define AlarmTurnOffPin D1
#define HeatingTurnOnPin D2
#define HeatingTurnOffPin D3

StaticJsonDocument<128> doc;
const char* ssid = "NETIASPOT-98C2";
const char* password = "7g*******p6";
const char* serverName0 = "http://192.168.0.136/inzynierka/post-mysql-functions.php";
const char* serverName1 = "http://192.168.0.136/inzynierka/TurnOnAlarm.json";
const char* serverName2 = "http://192.168.0.136/inzynierka/TurnOnHeating.json";
String TurnOnHeating_;
String TurnOnAlarm_;

void setup() {
  pinMode(AlarmTurnOnPin, OUTPUT);
  pinMode(AlarmTurnOffPin, OUTPUT);
  pinMode(HeatingTurnOnPin, OUTPUT);
  pinMode(HeatingTurnOffPin, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  }
}

void loop() {
  while (Serial.available() > 0) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName0);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = Serial.readString();
      int httpResponseCode = http.POST(httpRequestData);
      http.end();
 
      http.begin(client, serverName1);
      httpResponseCode = http.GET();
      String payload = http.getString();
      DeserializationError error = deserializeJson(doc, payload);
      const char* TurnOnAlarm = doc["TurnOnAlarm"];
      http.end();
      TurnOnAlarm_ = String(TurnOnAlarm);

      http.begin(client, serverName2);
      httpResponseCode = http.GET();
      payload = http.getString();
      error = deserializeJson(doc, payload);
      const char* TurnOnHeating = doc["TurnOnHeating"];
      http.end();
      TurnOnHeating_ = String(TurnOnHeating);

      if (TurnOnAlarm_ == "ON") {
        digitalWrite(AlarmTurnOnPin, HIGH);
        digitalWrite(AlarmTurnOffPin, LOW);
      }
      else if (TurnOnAlarm_ == "OFF") {
        digitalWrite(AlarmTurnOffPin, HIGH);
        digitalWrite(AlarmTurnOnPin, LOW);
      }
      if (TurnOnHeating_ == "ON") {
        digitalWrite(HeatingTurnOnPin, HIGH);
        digitalWrite(HeatingTurnOffPin, LOW);
        
      }
      else if (TurnOnHeating_ == "OFF") {
        digitalWrite(HeatingTurnOffPin, HIGH);
        digitalWrite(HeatingTurnOnPin, LOW);
       
      }
    }
  }
}