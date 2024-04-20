//Removing a namespace:


#include <nvs_flash.h> 
void setup() { 
Serial.begin(115200);
nvs_flash_erase(); // erase NVS partition
nvs_flash_init(); // init NVS partition. 
Serial.println();
Serial.printf("removing everything on non volatile storage");
while(true); 

}

void loop(){
  
}
