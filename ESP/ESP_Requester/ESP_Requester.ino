#include <ESP8266WiFi.h>

const char* ssid = "";     
const char* password = ""; 

void scanNetworks() {
    int numNetworks = WiFi.scanNetworks();
    
    if (numNetworks == 0) 
    {
        Serial.println("?");
    } 
    else 
    {
        Serial.println("!");
        for (int i = 0; i < numNetworks; i++) 
        {
            Serial.printf("%d: %s (Signal: %d dBm) %s\n", i + 1,
                          WiFi.SSID(i).c_str(), WiFi.RSSI(i),
                          WiFi.encryptionType(i) == ENC_TYPE_NONE ? "Open" : "Secured");
        }
    }
}

void connectToWiFi(String ssid, String password) {
    WiFi.begin(ssid.c_str(), password.c_str());

    Serial.print("Connecting to ");
    Serial.println(ssid);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) { // Try for 20 seconds
        delay(1000);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected!");
        Serial.print("ESP IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFailed to connect. Check SSID/Password.");
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);  // Set ESP as Wi-Fi client

    scanNetworks();  // Scan for available networks

    // *** User selects SSID & enters password here ***
    // Replace this with actual user input handling logic
    ssid = "User_SSID";      
    password = "User_Password";  

    connectToWiFi(ssid, password);
}

void loop() {
    // Your main code here after Wi-Fi is connected
}