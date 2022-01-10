const int buttonPin = 3;
const int motorPin =  2;

int buttonState = 0;  

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    pullCord();
  } else {
    digitalWrite(motorPin, LOW);
  }
}

void pullCord() {
  digitalWrite(motorPin, HIGH);
  delay(300);
  digitalWrite(motorPin, LOW);
}
