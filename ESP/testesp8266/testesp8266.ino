#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "1";
const char* password = "25102004";
const char* mqttServer = "test.mosquitto.org";
const char* requestTopic = "esp/request";    
const char* responseTopic = "esp/response";  

bool messageReceived = false;

WiFiClient wifiClient;
HTTPClient http;
PubSubClient client(wifiClient);
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
  client.setCallback(callback);
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
    else if (incomingChar == '!')
    {
      fetchGardenData();
      receivedUART = "";
    }
    else if (incomingChar == '#')  
    {
      scanAndSendNetworks();
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
  messageReceived = true;
  String gardenStateInfo;
  for (int i = 0; i < length; i++) 
  {
   gardenStateInfo += (char)payload[i];
  }
  Serial.println(gardenStateInfo + "!");
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
  messageReceived = false;
  client.setServer(mqttServer, 1883);
  if (client.connect("RequesterESP")) 
  {
    client.subscribe(responseTopic, 1);
    while (!messageReceived) 
    {
      client.publish(requestTopic, "?");
      unsigned long startTime = millis();
      while (millis() - startTime < 2000) 
      {
        client.loop();
        if (messageReceived)
        {
          break;
        } 
      }
    }
    client.disconnect();
  }
}

void scanAndSendNetworks() 
{
  int numOfNetworks = WiFi.scanNetworks();
  StaticJsonDocument<1024> doc;
  JsonArray networks = doc.createNestedArray("networks");

  for (int i = 0; i < numOfNetworks; ++i) 
  {
    JsonObject net = networks.createNestedObject();
    net["ssid"] = WiFi.SSID(i);
    net["secure"] = (WiFi.encryptionType(i) != ENC_TYPE_NONE);
  }

  serializeJson(doc, Serial);
  Serial.println("!");  
}