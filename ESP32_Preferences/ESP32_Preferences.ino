#include <Preferences.h>
#include <LiquidCrystal_I2C.h>

//illustrates how to use prefs

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

Preferences preferences; //declare prefs
void setup() { 
    Serial.begin(115200);
    Serial.println();

    // initialize LCD
    lcd.init();
    // turn on LCD backlight                      
    lcd.backlight();
    //open namespace, false means RW, true means R only
    preferences.begin("myNameSpace", false); 
    //read the value given a key, 0 is default
    unsigned int counter = preferences.getUInt("counter", 0);
    lcd.clear();
    // write values to lcd
    lcd.setCursor(0, 0);
    // print message
    lcd.print("curr cnt: ");
    lcd.setCursor(11, 0);
    lcd.print(String(counter));
    Serial.printf("Current counter value: %u\n", counter);
    counter++;
    // Store the counter to the Preferences 
    preferences.putUInt("counter", counter);
    // Close the Preferences
    preferences.end();
    lcd.setCursor(0, 1);
    lcd.print("Restarting");
    Serial.println("Restarting in 6 seconds...");
    delay(6000);
    // Restart ESP 
    ESP.restart();   
}

void loop(){
  
}
