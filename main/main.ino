const int buzzerPin = 4;
const int buttonPin = 3;                      
const int motorPin =  2;
const int OFF = 10;
const int TIMING = 20;
const int TIMEON = 30;
const int ANNOY = 40;
const int MOTORCTL = 50;

int lastButtonState = 0;
int buttonState = 0;
int state = OFF;
int minuteInterval = 1;
unsigned long currentTime = 0;
unsigned long waitTime = 0;
unsigned long selection = 0;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  currentTime = millis();
  //Serial.begin(9600);
}

void loop() {  
  switch(state){
    case OFF :
      buttonState = digitalRead(buttonPin);
      if (lastButtonState == HIGH){
        if (buttonState == LOW ){
          state = TIMING;
          waitTime = millis();
        }
      };
      lastButtonState = buttonState;
      break;
    case TIMING :
      if (digitalRead(buttonPin) == HIGH){
        soundOn();
        selection = 1;
        beep(900, 200);
        waitTime = millis();
        while ((millis() - waitTime) < 3000){
          if (digitalRead(buttonPin) == LOW){
            if (selection < 3) {
              selection++;
              for (unsigned long i = 0; i < selection; i++) {  // beep to indicate selection
                beep(900, 200);
                delay(100);
              }
              waitTime = millis();
            }
          }
          Serial.println(selection);
        }
        state = TIMEON;
      }
      else {
        if ((millis() - waitTime) > 1500){
          state = MOTORCTL;
        }
      }
      break;
    case TIMEON :
      soundTimeOn();

      // Option 1: delay() - should use less battery - TODO: look into sleep modes
      delay(selection * (unsigned long)15 * (unsigned long)1000 * (unsigned long)60);

      // Old method: get millis() - checks for button press to cancel - too power hungry
      /*waitTime = millis();
       *while ((millis() - waitTime) < (selection * (unsigned long)15 * (unsigned long)1000 * (unsigned long)60)){
        if (digitalRead(buttonPin) == LOW){
          beep(300, 200);
          goto exit_loop;
        }
        Serial.println(millis() - waitTime);
      }*/
      pullCord();  
      exit_loop:
      state = ANNOY;
    case ANNOY:
      while(true){
        beep(400, 200);
        delay(1000);
      };
    case MOTORCTL:
      if (digitalRead(buttonPin) == LOW){digitalWrite(motorPin, HIGH);}
      else {digitalWrite(motorPin, LOW);};
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
  delay(600);
  digitalWrite(motorPin, LOW);
}
