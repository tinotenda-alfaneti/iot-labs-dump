#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"

#define DHT22_PIN 19

//Wifi connection variables
const char* ssid = "MuAtarist";
const char* password = "123456ix";

//Domain name with URL path or IP address with path
const char* serverName = "http://192.168.163.170/iot/classactivity/dataCollectionApi.php";

const long dhtInterval = 1000;
int dhtStartTime = 0;

unsigned long lastTime = 0;
//time intervals
unsigned long timerDelay = 10000;

DHT dht22(DHT22_PIN, DHT22);

void setup() {
  Serial.begin(9600);
  dht22.begin(); // initialize the DHT11 sensor

  analogReadResolution(12);

  //connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print("-");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("It will take 1 seconds before publishing the first reading.");

}

void loop(){

    
  // DHT11 sensor readings
  if (millis() - dhtStartTime >= dhtInterval) {
    float humi  = dht22.readHumidity();
    float tempC = dht22.readTemperature();

   if ((millis() - lastTime) > timerDelay) {
    
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      float humi  = dht22.readHumidity();
      float tempC = dht22.readTemperature();
    
      // serverName=URL 
      http.begin(client, serverName);

      // For HTTP request with a content type: application/json:
      http.addHeader("Content-Type", "application/json");
      String jsonString = "{\"groupName\":\"" + String("NA") +
                     "\",\"temperatureVal\":\"" + String(tempC) +
                    "\",\"humidityVal\":\"" + String(humi) +
                    "\",\"pumpStatus\":\"" + String("NA") +
                    "\",\"collectionTime\":\"" + String("NA") + "\"}"; 
      Serial.println(jsonString);
      int httpResponseCode = http.POST(jsonString.c_str());
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  
}
