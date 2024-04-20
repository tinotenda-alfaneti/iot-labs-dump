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
#include <WebServer.h>
#include "htmltext.h"
#include <LiquidCrystal_I2C.h>

#define buzzer 13
#define pump 12
#define POWER_PIN  18 // ESP32 pin GPIO17 connected to sensor's VCC pin
#define SIGNAL_PIN 35 // ESP32 pin GPIO36 (ADC0) connected to sensor's signal pin

const int NUM_RECORDS = 10;

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

String mode = "AUTO";

String pumpStatus = "OFF";

String records[NUM_RECORDS];
long long currRecords = 0;

int oldestRecordIndex = 0; // Keep track of the oldest record index

int waterLevel = 0;

int readInterval = 2000;
int readStart = 0;


WebServer server(80);
char ssidAP[] = "ATAR";
char passwordAP[] = "psdX"; 
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
  server.on("/viewRecords",viewRecordsfunct);
  server.on("/changeMode",changeModefunct);
  server.on("/startPump",startPumpfunct);
  server.on("/updateVars",updateVarsfunct);

  pinMode(buzzer, OUTPUT);
  pinMode(pump, OUTPUT);
  analogReadResolution(12);

  pinMode(POWER_PIN, OUTPUT);   // configure pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW); // turn the sensor OFF

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();


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
    digitalWrite(pump, LOW);
    pumpStatus = "OFF";
  } else {
    digitalWrite(pump, HIGH);
    pumpStatus = "ON";
  }
  server.send(200, "text/plain", pumpStatus);
}

void updateVarsfunct(){
  server.send(200, "text/plain", pumpStatus);
}

void addRecord(int waterLevel) {
    
    // Update oldest record index if array is full
    if (currRecords == NUM_RECORDS) {
      for (int i = 0; i < NUM_RECORDS - 1; i++) {

        records[i] = records[i + 1];
        
      }
      currRecords--;
    }
        
    records[NUM_RECORDS - 1] = String(waterLevel);
    currRecords++;
}


void viewRecordsfunct() {
    String readingsJSON = "{\"readings\":[";// Read up to NUM_RECORDS or until all records are read
    for (int i = 0; i < NUM_RECORDS; i++) {// Calculate the index to read from
        readingsJSON += String(records[i]);
        if (i < NUM_RECORDS - 1) {
            readingsJSON += ",";
        }
    }
    readingsJSON += "]}";
    server.send(200, "application/json", readingsJSON);
}

void loop() {
  server.handleClient(); // manage HTTP requests
  //water sensor
  if (millis() - readStart >= readInterval) {
    digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
    delay(10);                      // wait 10 milliseconds
    waterLevel = analogRead(SIGNAL_PIN); // read the analog value from sensor
    addRecord(waterLevel);
    Serial.println(waterLevel);

    readStart = millis();

    if (waterLevel > 800 && mode == "AUTO") {
      digitalWrite(buzzer, HIGH);
      digitalWrite(pump, HIGH);
      pumpStatus = "ON";
    } else if (mode == "AUTO"){
      digitalWrite(buzzer, LOW);
      digitalWrite(pump, LOW);
      pumpStatus = "OFF";
    }
    lcd.clear();
    // write values to lcd
    lcd.setCursor(0, 0);
    // print message
    lcd.print("Water Lvl: ");
    // clears the display to print new message
    lcd.setCursor(11, 0);
    lcd.print(String(waterLevel));
  }
}
