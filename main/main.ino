const int buzzerPin = 4;
const int buttonPin = 3;                      
const int motorPin =  2;
const int OFF = 10;
const int TIMING = 20;
const int TIMEON = 30;

int lastButtonState = 0;
int buttonState = 0;
int state = OFF;
int minuteInterval = 1;
unsigned long currentTime = 0;
unsigned long waitTime = 0;
unsigned long remainingTime = 0;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  currentTime = millis();
  Serial.begin(9600);
}

void loop() {  
  switch(state){
    case OFF :
      buttonState = digitalRead(buttonPin);
      if (lastButtonState == HIGH){
        if (buttonState == LOW ){
          state = TIMING;
        }
      };
      lastButtonState = buttonState;
      break;
    case TIMING :
      if (digitalRead(buttonPin) == HIGH){
        remainingTime = minuteInterval * (unsigned long)1000 * (unsigned long)60;
        soundOn();
        waitTime = millis();
        while ((millis() - waitTime) < 3000){
          if (digitalRead(buttonPin) == LOW){
            beep(900, 200);
            remainingTime = (minuteInterval * (unsigned long)1000 * (unsigned long)60) + remainingTime;
            waitTime = millis();
          }
          Serial.println(remainingTime);
        }
        state = TIMEON;
      }
      break;
    case TIMEON :
      soundTimeOn();
      waitTime = millis();
      while ((millis() - waitTime) < remainingTime){
        if (digitalRead(buttonPin) == LOW){
          beep(300, 200);
          goto exit_loop;
        }
        Serial.println(millis() - waitTime);
      }
      pullCord();  
      exit_loop:
      state = OFF;
    default :
      ;
  }
}

void soundTimeOn(){
  for (int i = 500; i <= 1000; i=i+50) {
    beep(i, 100);
  }
}

void soundOn(){
  beep(400, 200);
  beep(700, 200);
  beep(900, 200);
  delay(500);
  beep(900, 200);
}

void beep(int level, int duration){
  tone(buzzerPin, level);
  delay(duration);
  noTone(buzzerPin);
}

void soundWarning(){
  beep(900, 200);
  beep(700, 200);
  beep(400, 200);
  delay(500);
}

void pullCord() {
  soundWarning();
  digitalWrite(motorPin, HIGH);
  delay(300);
  digitalWrite(motorPin, LOW);
}
