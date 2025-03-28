#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "1";
const char* password = "25102004";
const char* mqttServer = "test.mosquitto.org";
const char* requestTopic = "esp/request";    
const char* responseTopic = "esp/response";  

WiFiClient wifiClient;
PubSubClient client(espClient);
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
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop() 
{  
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
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

void callback(char* topic, byte* payload, unsigned int length) 
{
  String message;
  for (int i = 0; i < length; i++) 
  {
   message += (char)payload[i];
  }
  Serial.println("Received Sensor Data: " + message);
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


void reconnect() 
{
  while (!client.connected()) 
  {
    if (client.connect("RequesterESP")) 
    {
      Serial.println("Connected to MQTT!");
      client.subscribe(responseTopic);
    } 
    else 
    {
      delay(2000);
    }
  }
}

void fetchGardenData()
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.publish(requestTopic, "?");
}
