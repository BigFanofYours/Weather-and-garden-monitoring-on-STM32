#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = "1";
const char* password = "25102004";
const char* mqttServer = "test.mosquitto.org";
const char* requestTopic = "esp/request";    
const char* responseTopic = "esp/response";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

#define DHTPIN 2  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

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
  if (client.connect("SensorESP"))
  {
    client.subscribe(requestTopic, 1);
  }
  dht.begin();
}

void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  String message;
  for (int i = 0; i < length; i++) 
  {
    message += (char)payload[i];
  }

  if (message == "?") 
  {
    float gardenTemperature = dht.readTemperature();
    float gardenHumidity = dht.readHumidity();
    String data = "{\"gardenTemperature\":" + String(gardenTemperature) + ",\"gardenHumidity\":" + String(gardenHumidity) + "}";
    client.publish(responseTopic, data.c_str());
  }
}

void reconnect() 
{
  while (!client.connected()) 
  {
    if (client.connect("SensorESP")) 
    {
      Serial.println("Connected to MQTT!");
      client.subscribe(requestTopic, 1);
    } 
    else 
    {
      delay(2000);
    }
  }
}