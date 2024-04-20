const int buzzer = 18;
unsigned long startTime= 0;
const long interval = 10000;

void setup() {
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  Serial.printf("Alarm started...\n");
}

void loop() {
  unsigned long currentTime= millis();
  if (currentTime - startTime >= interval) {
    playSound(2000); 
    startTime= currentTime;
  }
}

void playSound(int duration) {
  for (int i = 0; i < duration/500; i++) {
    tone(buzzer, 1000);
    delay(500);
    noTone(buzzer);
    delay(500);
  }
}

