#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid = "1";
const char* password = "25102004";
const char* serverURL = "http://127.0.0.1:5000/request?q?";
WiFiClient wifiClient;
HTTPClient http;
String receivedUART = "";

void setup() 
{
  Serial.begin(9600);
  delay(5000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
}

void loop() 
{  
  if (Serial.available()) 
  {
    char incomingChar = Serial.read();
    if (incomingChar == '\n') 
    {
      String weatherData = fetchWeatherData(receivedUART);
      Serial.println(weatherData + "!");
      receivedUART = "";
    }
    else if (incomingChar == '?')
    {
      fetchGardenData();
      receivedUART = "";
    }
    else 
    {
      receivedUART += incomingChar;
    }
  }
}

String trimWhiteSpace(String input) 
{
  input.trim(); 
  return input;
}

String fetchWeatherData(String URL) 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    URL = trimWhiteSpace(URL);
    http.begin(wifiClient, URL); 
    int httpCode = http.GET(); 
    if (httpCode == HTTP_CODE_OK) 
    {
      String payload = http.getString();
      http.end();
      return payload;
    }  
    http.end();
  } 
  return "{}?";
}

void fetchGardenData()
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    http.begin(wifiClient, serverURL);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) 
    {
      String payload = http.getString();
      http.end();
      Serial.println(payload);  
    }  
    else 
    {
      Serial.println("Failed to get data");
    }
    http.end();
  }
}
