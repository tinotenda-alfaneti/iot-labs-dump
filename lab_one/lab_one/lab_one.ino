
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"

#define DHT22_PIN 5 
#define yellowPin 21
#define redPin 22
#define bluePin 18
#define TEMP 30
#define ldrPin 34

//Wifi connection variables
const char* ssid = "MuAtarist";
const char* password = "123456ix";

// for current time and date
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;   //Replace with your GMT offset (seconds)
const int   daylightOffset_sec = 0;

//Domain name with URL path or IP address with path
const char* serverName = "http://192.168.205.170/iot/labOne/sensordata_api.php";

const long dhtInterval = 3000;
int dhtStartTime = 0;

const int yellowLEDInterval = 600;
int yellowLEDStartTime = 0;

const int ldrInterval = 1500;
int ldrStartTime = 0;

unsigned long lastTime = 0;
//time intervals
unsigned long timerDelay = 10000;

DHT dht22(DHT22_PIN, DHT22);

void setup() {
  Serial.begin(9600);
  dht22.begin(); // initialize the DHT11 sensor

  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(bluePin, OUTPUT);

  analogReadResolution(12);

  //connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("It will take 5 seconds before publishing the first reading.");
    //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {

  int ldrReading = analogRead(ldrPin);

  //dark condition
  if (ldrReading > 4000) digitalWrite(bluePin, HIGH);
  else digitalWrite(bluePin, LOW);

  //ldr after every 1.5 seconds
  if (millis() - ldrStartTime >= ldrInterval) {
    Serial.print("Millis: ");
    Serial.print(millis());

    Serial.print("  |  ");

    Serial.print("LDR Reading: ");
    Serial.println(ldrReading);
    ldrStartTime = millis();
  }
  //yellow led after every 600ms
  if (millis() - yellowLEDStartTime >= yellowLEDInterval) {
    
    int ledState = !digitalRead(yellowPin);
    digitalWrite(yellowPin, ledState);
    Serial.print("Millis: ");
    Serial.print(millis());
    Serial.print("  |  ");
    if (ledState) Serial.println("Yellow LED turned ON");
    else Serial.println("Yellow LED turned OFF");
    yellowLEDStartTime = millis();
  }

  // DHT11 sensor readings
  if (millis() - dhtStartTime >= dhtInterval) {
    float humi  = dht22.readHumidity();
    float tempC = dht22.readTemperature();

    // check whether the reading is successful or not
    if ( isnan(tempC) || isnan(humi)) {
      Serial.println("Failed to read from DHT11 sensor!");
    } else {
      if (tempC > TEMP) digitalWrite(redPin, HIGH);
      else digitalWrite(redPin, LOW);
      Serial.print("Millis: ");
      Serial.print(millis());

      Serial.print("  |  ");

      Serial.print("Humidity: ");
      Serial.print(humi);
      Serial.print("%");

      Serial.print("  |  ");

      Serial.print("Temperature: ");
      Serial.print(tempC);
      Serial.println("°C");
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
      String sensorType = "Temperature";
      String sensorName = "DHT22 Sensor";
      String sensorLocation = "A16";
      String dateRead = getLocalDate();
      String timeRead = getLocalTime();
      
      // For HTTP request with a content type: application/json:
      http.addHeader("Content-Type", "application/json");
      String jsonString = "{\"SensorName\":\"" + sensorName +
                    "\",\"SensorType\":\"" + sensorType +
                    "\",\"CurrentReading\":\"" + String(tempC) +
                    "\",\"SensorLocation\":\"" + sensorLocation +
                    "\",\"DateRead\":\"" + dateRead +
                    "\",\"TimeRead\":\"" + timeRead + "\"}"; 
      int httpResponseCode = http.POST(jsonString.c_str());
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // take humidity
      sensorType = "Humidity";
      sensorName = "DHT22 Sensor";
      sensorLocation = "A16";
      dateRead = getLocalDate();
      timeRead = getLocalTime();
      
      // For HTTP request with a content type: application/json:
      http.addHeader("Content-Type", "application/json");
      jsonString = "{\"SensorName\":\"" + sensorName +
                    "\",\"SensorType\":\"" + sensorType +
                    "\",\"CurrentReading\":\"" + String(humi) +
                    "\",\"SensorLocation\":\"" + sensorLocation +
                    "\",\"DateRead\":\"" + dateRead +
                    "\",\"TimeRead\":\"" + timeRead + "\"}"; 
      httpResponseCode = http.POST(jsonString.c_str());
     
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

String getLocalDate() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "Failed to obtain time";
  }

  char buffer[100]; // Adjust the buffer size based on your formatting needs
  strftime(buffer, sizeof(buffer), "%A, %B %d %Y", &timeinfo);

  return String(buffer);
}


String getLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "Failed to obtain time";
  }

  char buffer[100]; // Adjust the buffer size based on your formatting needs
  strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);

  return String(buffer);
}

