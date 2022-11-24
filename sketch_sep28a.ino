#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>





const char* ssid = "NETIASPOT-98C2";
const char* password = "7g4D6UcG6qp6";
const char* serverName = "http://mts.wibro.agh.edu.pl/~s403062/projekt_inzynierski/esp-post-data.php";
// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /esp-post-data.php also needs to have the same key
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
 http.begin(client, serverName);
// Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
     String httpRequestData = Serial.readString();
     Serial.print(httpRequestData);
      int httpResponseCode = http.POST(httpRequestData);

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");

      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
http.end();
    }
     }
  
}

