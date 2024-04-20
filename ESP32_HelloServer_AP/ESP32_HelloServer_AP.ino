/**
 * based on code by Niel Cameron
 * Setup: connect a RED led to pin 21 and a green LED to pin 23
 * YOu may rename your Acess point from ESP32Svr to make it identifiable and uniqe
 * You may change your IP if you wish, but not neccessary.
 * Only difference betwen this an STA version is in the setup of the wireless
 */


#include <WebServer.h>
WebServer server(80);
int LEDGpin = 22; /*green*/
int LEDRpin = 21; /*red*/
int LEDBpin = 19; /*blue*/
  
#include "htmltext.h"

char ssidAP[] = "KTE_CS";
char passwordAP[] = "12345678"; 
IPAddress local_ip(192,168,2,1); 
IPAddress gateway(192,168,2,1); 
IPAddress subnet(255,255,255,0); 

int LEDR = LOW;
int LEDG = LOW;
int LEDB = LOW;
int counter = 0;
void setup()
{
  WiFi.mode(WIFI_AP);
  delay(1000);
  WiFi.softAP(ssidAP, passwordAP); 
  WiFi.softAPConfig(local_ip, gateway, subnet);  // initialise Wi-Fi 
  server.begin();
  server.on("/", base);
  server.on("/LEDGurl",LEDGfunct);
  server.on("/LEDRurl",LEDRfunct);
  server.on("/LEDBurl", LEDBfunct);
  server.on("/zeroUrl",zerofunct);
  pinMode(LEDGpin, OUTPUT);
  pinMode(LEDRpin, OUTPUT);
  pinMode(LEDBpin, OUTPUT);
}

void base(){
  server.send(200, "text.html", webcode(LEDG, LEDR, LEDB, counter));
}

void LEDGfunct(){
  LEDG = !LEDG;
  digitalWrite(LEDGpin, LEDG);
  counter++;
  server.send(200, "text/html", webcode(LEDG, LEDR, LEDB, counter));
}

void LEDRfunct(){
  LEDR = !LEDR;
  digitalWrite(LEDRpin, LEDR);
  counter++;
  server.send(200, "text/html", webcode(LEDG, LEDR, LEDB, counter));
}
void LEDBfunct(){
  LEDB = !LEDB;
  digitalWrite(LEDBpin, LEDB);
  counter++;
  server.send(200, "text/html", webcode(LEDG, LEDR, LEDB, counter));
}

void zerofunct() // function to zero counter
{
  // and send HTML code to client
  counter = 0;
  server.send(200, "text/html", webcode(LEDG, LEDR, LEDB, counter));
}
void loop() {
  server.handleClient(); // manage HTTP requests 
}
