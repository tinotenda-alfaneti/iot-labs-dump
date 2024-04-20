#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>
//char ssid[] = "NNA phone14";
//char password[] = "nathan1234000";

char ssid[] = "MuAtarist";
char password[] = "123456ix";

int LEDpin = 2;
int blueLED = 18;

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 




void setup() {
  Serial.begin(115200);
  pinMode(LEDpin, OUTPUT);
  pinMode(blueLED, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    flash();
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  ArduinoOTA.setHostname("ATARIST_ESP32");
  ArduinoOTA.setPassword("konekt");
  ArduinoOTA.begin();   // initialise ArduinoOTA

  
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

}


void loop() {
  ArduinoOTA.handle();
  digitalWrite(LEDpin, !digitalRead(LEDpin)); // toggle
  delay(200);
  digitalWrite(blueLED, !digitalRead(blueLED));
  lcd.clear();
  // write values to lcd
  lcd.setCursor(0, 0);
  // print message
  lcd.print("OTA mode");
}


void flash()      //only flash quickly at while connecting
{
    digitalWrite(LEDpin, HIGH);
    delay(100);
    digitalWrite(LEDpin, LOW);
}
