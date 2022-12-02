#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

StaticJsonDocument<128> doc;
const char* ssid = "NETIASPOT-98C2";
const char* password = "7g4D6UcG6qp6";
const char* serverName0 = "http://192.168.0.136/inzynierka/esp-post-data.php";
//const char* serverName2 = "http://mts.wibro.agh.edu.pl/~s403062/projekt_inzynierski/JSONtoReturn.php";
const char* serverName1 = "http://192.168.0.136/inzynierka/TurnOnAlarm.json";
const char* serverName2 = "http://192.168.0.136/inzynierka/TurnOnHeating.json";
const char* serverName3 = "http://192.168.0.136/inzynierka/SetTemperature.json";

String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "DHT11";
String sensorLocation = "Home";
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;
int TurnOnHeating_value=0;
int TurnOnAlarm_value=0;
int SetTemperature_value=0;
String TurnOnHeating_;
String TurnOnAlarm_;
String SetTemperature_;
int SetTemperature__;
int toSend;
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
 http.begin(client, serverName0);
// Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
     String httpRequestData = Serial.readString();
     //Serial.print(httpRequestData);
      int httpResponseCode = http.POST(httpRequestData);
//JSON 


http.end();      


http.begin(client, serverName1);
httpResponseCode=http.GET();
String payload = http.getString();
DeserializationError error = deserializeJson(doc, payload);
const char* TurnOnAlarm = doc["TurnOnAlarm"]; // "yes"
http.end();
TurnOnAlarm_=String(TurnOnAlarm);

http.begin(client, serverName2);
httpResponseCode=http.GET();
 payload = http.getString();
 error = deserializeJson(doc, payload);
const char* TurnOnHeating = doc["TurnOnHeating"]; // "yes"
http.end();
TurnOnHeating_=String(TurnOnHeating);

http.begin(client, serverName3);
httpResponseCode=http.GET();
 payload = http.getString();
 error = deserializeJson(doc, payload);
const char* SetTemperature = doc["SetTemperature"]; // "yes"
http.end();
SetTemperature_=String(SetTemperature);


 SetTemperature__=SetTemperature_.toInt();


if(TurnOnAlarm_=="ON"){
  TurnOnAlarm_value=1000;
  
  
}else{
  TurnOnAlarm_value=0;
 }
 if(TurnOnHeating_=="ON"){
  TurnOnHeating_value=100;
  }else{
  TurnOnHeating_value=0;
 }
toSend= SetTemperature__+TurnOnAlarm_value+TurnOnHeating_value;

Serial.print(String(toSend));
Serial.flush();


//http.end();
}
}
}
