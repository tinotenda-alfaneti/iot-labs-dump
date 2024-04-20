

#include <LiquidCrystal_I2C.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include "htmltext.h"

#define RECORDS 10

////Domain name with URL path or IP address with path
const char* serverName = "http://192.168.88.21/IOT/Lab_three/php_files/dataCollectionApi.php";

WebServer server(80);

char ssidAP[] = "Thess";
char passwordAP[] = "12341111"; 
IPAddress local_ip(192, 168, 2, 1); 
IPAddress gateway(192, 168, 2, 1); 
IPAddress subnet(255, 255, 255, 0); 

#define buzzer 18
#define WaterSensor 35
#define POWER_PIN 25
int PUMPpin = 23; // led pin

int PUMP = LOW;
int value = 0;
String mode = "AUTO";
int sensorReadings[10];
int currentIndex = 0;
int currRecords = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  delay(1000);
  WiFi.softAP(ssidAP, passwordAP); 
  WiFi.softAPConfig(local_ip, gateway, subnet);  // Initialise Wi-Fi 

  server.begin();
  server.on("/", base);
  server.on("/viewTen",lastTen);
  server.on("/pumpState", getPumpState);
  server.on("/changeMode", changeModeFxn);
  server.on("/pumping", turnPump);

  pinMode(POWER_PIN, OUTPUT);   // configure pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW); // turn the sensor OFF
  pinMode(buzzer, OUTPUT);
  pinMode(WaterSensor, INPUT);
  pinMode(PUMPpin, OUTPUT);
  lcd.init(); // Initialise the LCD
  lcd.backlight(); 
}

void base() {
  server.send(200, "text/html", page);
}


void changeModeFxn() {
  if (mode == "AUTO") mode = "MANUAL";
  else mode = "AUTO";
  server.send(200, "text/plain", mode);
  }

void getPumpState() {
  int currPumpState = digitalRead(PUMPpin);
 String str = "ON"; // Default to ON
  if(currPumpState == LOW) str = "OFF";
 server.send(200, "text/plain", str);
  Serial.println("The pump is " + str);
}

void turnPump() {
  
    digitalWrite(buzzer, !digitalRead(buzzer));
    digitalWrite(PUMPpin, !digitalRead(PUMPpin));
    server.send(200, "text/plain", String(digitalRead(PUMPpin)));
  
}

void LCD_write() {
  lcd.clear();
  lcd.setCursor(0, 0);   // Move cursor to (0, 0)
  lcd.print("Water Level: "); // Print message at (0, 0)
  lcd.print(value);    // Print message at (2, 1)
   Serial.println("Water level: ");
    Serial.println(value);
}

void SenseWater() {
  LCD_write();
  if (mode == "AUTO") {
    if (value == 0) {
      digitalWrite(buzzer, LOW);
      digitalWrite(PUMPpin, LOW);
    } else if (value > 0 && value <100){
      digitalWrite(buzzer, HIGH);
      digitalWrite(PUMPpin, HIGH);
  //    handlePump();
    } else {
      digitalWrite(buzzer, LOW);
      digitalWrite(PUMPpin, LOW);
    }
  }
}




void lastTen(){

    String payload = "{\"readings\":[";// Read up to NUM_RECORDS or until all records are read
    for (int i = 0; i < RECORDS; i++) {// Calculate the index to read from
        payload += String(sensorReadings[i]);
        if (i < RECORDS - 1) {
            payload += ",";
        }
    }
    payload += "]}";
    Serial.println(payload)
    server.send(200, "application/json", payload);

}


void loop() {
  server.handleClient();
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(1000);                      // wait 10 milliseconds
  value = analogRead(WaterSensor);
  digitalWrite(POWER_PIN, LOW);
  sensorReadings[currRecords % 10] = value;
  currRecords++;
  Serial.print("Mode: ");
  Serial.println(mode); 
  SenseWater();
}
