

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


#include <Wire.h>
#include <Adafruit_Sensor.h>


const char* ssid = "NETIASPOT-98C2";
const char* password = "7g4D6UcG6qp6";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://mts.wibro.agh.edu.pl/~s403062/projekt_inzynierski/esp-post-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /esp-post-data.php also needs to have the same key
String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "BME280";
String sensorLocation = "Office";
String temp="";
String hum="";
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/




//Adafruit_BME280 bme(BME_CS);  // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 30 seconds (30000)
unsigned long timerDelay = 30000;

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
 // Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
 // Serial.println("");
  //Serial.print("Connected to WiFi network with IP Address: ");
 // Serial.println(WiFi.localIP());

  // (you can also pass in a Wire library object like &Wire2)

}

void loop() {
  //Send an HTTP POST request every 10 minutes
  while (Serial.available()>0){
          temp=String(Serial.read());
          Serial.print(temp);
          hum=String(Serial.read());
          Serial.print(hum);
      }
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                            + "&location=" + sensorLocation + "&value1=" + temp
                            + "&value2=" + hum + "&value3=" + String(Serial.read()) + "";
      //Serial.print("httpRequestData: ");
      //Serial.println(httpRequestData);

      // You can comment the httpRequestData variable above
      // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
      //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");

      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

      if (httpResponseCode>0) {
        //Serial.print("HTTP Response code: ");
        //Serial.println(httpResponseCode);
      }
      else {
        //Serial.print("Error code: ");
        //Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      //Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
