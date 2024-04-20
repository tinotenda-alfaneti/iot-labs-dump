/**
 * based on code by Niel Cameron
 * Setup: connect a RED led to pin 21 and a green LED to pin 23
 * YOu may rename your Acess point from ESP32Svr to make it identifiable and uniqe
 * You may change your IP if you wish, but not neccessary.
 * Only difference betwen this an STA version is in the setup of the wireless
 */
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"
#include <WebServer.h>
#include "htmltext.h"

#define DHT22_PIN 19
#define buzzer 13
#define TEMP 35

//Wifi connection variables
const char* ssid = "CHARLOTTE WIFI";
const char* password = "charlotte@12345";

// for current time and date
const char* ntpServer = "pool.ntp.org";

const long  gmtOffset_sec = 0;   //Replace with your GMT offset (seconds)
const int   daylightOffset_sec = 0;

//Domain name with URL path or IP address with path
const char* serverName = "http://192.168.88.21/iot/classactivity/dataCollectionApi.php";

const long dhtInterval = 3000;
int dhtStartTime = 0;

unsigned long lastTime = 0;
//time intervals
unsigned long timerDelay = 10000;

// pump control
String pumpStatus = "OFF";
int pumpState  = 0;
String mode = "AUTO";

DHT dht22(DHT22_PIN, DHT22);

WebServer server(80);
char ssidAP[] = "KTE_CS";
char passwordAP[] = ""; 
IPAddress local_ip(192,168,2,1); 
IPAddress gateway(192,168,2,1); 
IPAddress subnet(255,255,255,0); 


void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  delay(1000);
  WiFi.softAP(ssidAP, passwordAP); 
  WiFi.softAPConfig(local_ip, gateway, subnet);  // initialise Wi-Fi 
  server.begin();
  server.on("/", base);
  server.on("/viewTen",getLastTen);
  server.on("/changeMode",changeModefunct);
  server.on("/startPump",startPumpfunct);
  server.on("/getdata", getData);
  Serial.begin(9600);
  dht22.begin(); // initialize the DHT11 sensor

  pinMode(buzzer, OUTPUT);
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
    //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);


}

void base(){
  server.send(200, "text/html", page);
}

void changeModefunct() {

  if (mode == "AUTO") mode = "MANUAL";
  else mode = "AUTO";
  server.send(200, "text/plain", mode);

}

void startPumpfunct(){
  if (pumpStatus == "ON") {
    digitalWrite(buzzer, LOW);
    pumpStatus = "OFF";
  } else {
    digitalWrite(buzzer, HIGH);
    pumpStatus = "ON";
  }
  server.send(200, "text/plain", pumpStatus);
}



void postDataToDB() {
  // DHT11 sensor readings
  if (millis() - dhtStartTime >= dhtInterval) {
    float humi  = dht22.readHumidity();
    float tempC = dht22.readTemperature();

    // check whether the reading is successful or not
    if ( isnan(tempC) || isnan(humi)) {
      Serial.println("Failed to read from DHT11 sensor!");
    } 
    else if (mode == "AUTO") {
      if(tempC > TEMP){
        digitalWrite(buzzer, HIGH);
        pumpStatus = "ON";
        
      }
      else {
        digitalWrite(buzzer, LOW);
        pumpStatus = "OFF";
        };

    }

    dhtStartTime = millis();

  }

   if ((millis() - lastTime) > timerDelay) {
    
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      float humi  = dht22.readHumidity();
      float tempC = dht22.readTemperature();
    
      // serverName=URL 
      http.begin(client, serverName);
      String groupName = "TinoAndThessa";
      String dateRead = getLocalDateTime();

      // For HTTP request with a content type: application/json:
      http.addHeader("Content-Type", "application/json");
      String jsonString = "{\"groupName\":\"" + groupName +
                    "\",\"temperatureVal\":\"" + String(tempC) +
                    "\",\"humidityVal\":\"" + String(humi) +
                    "\",\"pumpStatus\":\"" + pumpStatus +
                    "\",\"collectionTime\":\"" + dateRead + "\"}"; 
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

String getLocalDateTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "Failed to obtain time";
  }

  char buffer[100]; // Adjust the buffer size based on your formatting needs
  strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", &timeinfo);

  return String(buffer);
}

void getLastTen(){
  //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      String serverPath = "http://192.168.88.21/iot/classactivity/dataCollectionApi.php/records";      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());  //alt use char host[] = "example.com";
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        server.send(200, "text/html", payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}

void getData(){
  //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;      
      // Your Domain name with URL path or IP address with path
      http.begin(serverName);  //alt use char host[] = "example.com";
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        server.send(200, "text/html", payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}

void loop() {
  server.handleClient(); // manage HTTP requests 
  postDataToDB();
}
