#include <ESP8266WiFi.h>

const char* ssid = "";     
const char* password = ""; 

void scanNetworks() 
{
  int numNetworks = WiFi.scanNetworks();
  for (int i = 0; i < numNetworks; i++) 
  {
    Serial.printf("%d: %s %s\n", i + 1,
                  WiFi.SSID(i).c_str(), 
                  WiFi.encryptionType(i) == ENC_TYPE_NONE ? "Open" : "Secured");
  }
}

void connectToWiFi(String ssid, String password) 
{
  WiFi.begin(ssid.c_str(), password.c_str());

  int attempts = 0; 
  while (WiFi.status() != WL_CONNECTED && attempts < 20) 
  { 
    delay(1000);
    attempts++;
  }
}

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  

  scanNetworks();  

  ssid = "User_SSID";      
  password = "User_Password";  

  connectToWiFi(ssid, password);
}

void loop() 
{

}