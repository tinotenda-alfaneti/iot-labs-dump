
int red = 18;
int yellow = 19;
int green = 17;
int button = 16;
unsigned long redStart = 0;
unsigned long yellowStart = 0;
const long redInterval = 300;
const long yellowInterval = 500;
int buttonState = 0;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);
  Serial.printf("Started...\n");

}

void loop() {
  
  unsigned long currentTime = millis();
  buttonState = digitalRead(button);

  if (currentTime - redStart >= redInterval) {
    int ledState = !digitalRead(red);
    digitalWrite(red, ledState);
    redStart = currentTime;
  }
  if (currentTime - yellowStart >= yellowInterval) {
    int ledState = !digitalRead(yellow);
    digitalWrite(yellow, ledState);
    yellowStart = currentTime;
  }

  if (buttonState == HIGH) {
    digitalWrite(green, HIGH);
  } else {
    digitalWrite(green, LOW);
  }
  delay(10);

}
