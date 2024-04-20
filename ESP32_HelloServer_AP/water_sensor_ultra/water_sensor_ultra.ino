#include <LiquidCrystal_I2C.h>
const int trigPin = 18;
const int echoPin = 19;
const int power_pin = 16;
const int signal_pin = 34;

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

long duration;
int distance;
int value = 0; 

void setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(power_pin, OUTPUT);   // configure pin as an OUTPUT
  digitalWrite(power_pin, LOW);
    lcd.init();
  // turn on LCD backlight                      
  lcd.backlight(); 
  Serial.begin(9600);
}

void loop(){
  //read water level
  digitalWrite(power_pin, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  value = analogRead(signal_pin); // read the analog value from sensor
  digitalWrite(power_pin, LOW);   // turn the sensor OFF

  Serial.print("The water sensor value: ");
  Serial.println(value);

  lcd.setCursor(0, 0);
  // print message
  lcd.print(value);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034/2;

  Serial.println("Distance: ");
  Serial.println(distance);
  lcd.setCursor(0, 1);
  // print message
  lcd.print(distance);
}