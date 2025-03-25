#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
const char* ssid = "1";
const char* password = "25102004";
WiFiClient wifiClient;
#define DHTPIN 2  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

ESP8266WebServer server(80);

void setup() 
{
  Serial.begin(9600);
  delay(5000);
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  Serial.println("\nConnected to WiFi");

  server.on("/request", HTTP_GET, handleRequest);
  server.begin();
}

void loop() 
{
  server.handleClient();
}

void handleRequest() 
{
  if (server.arg("cmd") == "?") 
  {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    String response = "{\"humidity\":" + String(humidity) + ", \"temperature\":" + String(temperature) + "}";
    server.send(200, "application/json", response);
  } 
  else 
  {
    server.send(400, "application/json", "{\"error\":\"Invalid command\"}");
  }
}