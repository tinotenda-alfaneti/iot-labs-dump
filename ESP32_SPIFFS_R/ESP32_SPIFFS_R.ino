#include "SPIFFS.h"
void setup() { 
 Serial.begin(115200); 
 if(!SPIFFS.begin(true)){
   Serial.println("An Error has occurred while mounting SPIFFS");
   return; 
 }
 File file = SPIFFS.open("/demo.txt");
 if(!file){
    Serial.println("Failed to open file for reading"); 
    return; 
 }
 Serial.println("File Content:"); 
 while(file.available()){
  Serial.write(file.read());
 } 
file.close();
}
void loop() { }
