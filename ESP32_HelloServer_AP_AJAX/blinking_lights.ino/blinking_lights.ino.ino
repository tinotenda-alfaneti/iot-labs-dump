//choose appropriate GPIO numbers
byte pin2=21; //
byte pin3=22;
byte pin4=23;
// the setup function runs once when you 
// press reset or power the board
void setup() {
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
}

// the loop function runs over and over again forever
void loop() {
  //traffic lights
  digitalWrite(pin4, LOW);
  digitalWrite(pin2, HIGH);
  delay(2000);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, HIGH);
  delay(500);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
  delay(2000);
}
