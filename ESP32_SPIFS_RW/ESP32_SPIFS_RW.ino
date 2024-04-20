//writes 10 numbers, one each second, and then reads them back.
#include "SPIFFS.h"
void setup() { 
 Serial.begin(115200); 
 if(!SPIFFS.begin(true)){
   Serial.println("An Error has occurred while mounting SPIFFS");
   return; 
 }
 File file = SPIFFS.open("/theFileName.txt", "w");
 if(!file){
    Serial.println("Failed to open file for writing"); 
    return; 
 }
 //write...
  Serial.println("Writing to SPIFFS");
  int bytesWritten = file.print("TEST SPIFFS"); 
  if (bytesWritten > 0) {
    Serial.print("File was written. Bytes written: ");
    Serial.println(bytesWritten);
 
  } else {
    Serial.println("File write failed");
    return;
  }

int count=0;  //write 10x
int start=0;
int current =millis();
while (count <10){
  if ((current-start) > 1000){
    start=current;
    count++;
    bytesWritten = file.println(String(count)+","); 
    //the following are also valid
    //bytesWritten = file.println(String(count)+",");
    //bytesWritten = file.print(String(count));  
    if (!(bytesWritten >0)){
      Serial.println("failed to write..halting");
      file.close();
      while(1);
    }
    else
      Serial.print(".");
   
  }
   current =millis();
}//while
  Serial.println("finished writing");
 
  file.close();


 //read
 file = SPIFFS.open("/theFileName.txt");
 Serial.println("File Content:"); 
 while(file.available()){
  Serial.write(file.read());
 } 
file.close();
}
void loop() { }
