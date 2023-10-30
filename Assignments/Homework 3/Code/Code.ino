const int ledLevel1 = 11;
const int ledLevel2 = 10;
const int ledLevel3 = 9;
const int ledGreen = 7;
const int buttonLevel1 = 4;
const int buttonLevel2 = 3;
const int buttonLevel3 = 2;
const int buzzer = 6;

byte led1State = HIGH;
byte led2State = LOW;
byte led3State = LOW;
byte ledGreenState = HIGH;

byte reading1 = LOW;
byte reading2 = LOW;
byte reading3 = LOW;
byte lastReading1 = LOW;
byte lastReading2 = LOW;
byte lastReading3 = LOW;

unsigned int lastDebounce1 = 0;
unsigned int lastDebounce2 = 0;
unsigned int lastDebounce3 = 0;
unsigned int debounceDelay = 50;
unsigned int lastDebounceGreen = 0;
unsigned int debounceDelayGreen = 100;
unsigned int lastTransition = 0;
unsigned int transitionDelay = 1000;

int currentLevel = 1;
int targetLevel = 1;
int buzzerTone = 1000;

void setup() {
  pinMode(ledLevel1, OUTPUT);
  pinMode(ledLevel2, OUTPUT);
  pinMode(ledLevel3, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(buttonLevel1, INPUT_PULLUP);
  pinMode(buttonLevel2, INPUT_PULLUP);
  pinMode(buttonLevel3, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  LightLeds();

  // daca liftul este in miscare, nu se poate citi alt buton care sa intrerupa procesul
  if (currentLevel == targetLevel)
  {
    ledGreenState = HIGH;
    lastTransition = millis();
    ReadButtons();
  }
  else
  {
    ElevatorElevatingSound();
    GreenLedBlink();
    NextLevel();
    CurrentLedLight();
  }
}

void ReadButtons()
{  
    // citeste butonul 1
    reading1 = digitalRead(buttonLevel1);
    if (reading1 != lastReading1) {
      lastDebounce1 = millis();
    }
    if ((millis() - lastDebounce1) > debounceDelay) {
      targetLevel = 1;
      lastReading1 = reading1;

    }

    // citeste butonul 2
    reading2 = digitalRead(buttonLevel2);
    if (reading2 != lastReading2) {
      lastDebounce2 = millis();
    }
    if ((millis() - lastDebounce2) > debounceDelay) {
      targetLevel = 2;
      lastReading2 = reading2;
    }

    // citeste butonul 3
    reading3 = digitalRead(buttonLevel3);
    if (reading3 != lastReading3) {
      lastDebounce3 = millis();
    }
    if ((millis() - lastDebounce3) > debounceDelay) {
      targetLevel = 3;
      lastReading3 = reading3;
    }
  // Serial.println(targetLevel);
}

void LightLeds(){
  digitalWrite(ledLevel1, led1State);
  digitalWrite(ledLevel2, led2State);
  digitalWrite(ledLevel3, led3State);
  digitalWrite(ledGreen, ledGreenState);
}

void CurrentLedLight()
{
    // Turn off all LEDs
    led1State = LOW;
    led2State = LOW;
    led3State = LOW;

    // Turn on the LED corresponding to the current level
    if (currentLevel == 1)
    {
      led1State = HIGH;
    }
    else if (currentLevel == 2)
    {
      led2State = HIGH;
    }
    else if (currentLevel == 3)
    {
      led3State = HIGH;
    }
}

void NextLevel()
{
  // create a delay when changing levels
  if(millis() - lastTransition < transitionDelay)
    return;
  lastTransition = millis();

  // go to next level
  if (currentLevel < targetLevel) currentLevel += 1;
  else currentLevel -= 1;

  // check if elevator arrived
  if(currentLevel == targetLevel){
    ElevatorArrivesSound();
  }
}

void GreenLedBlink(){
    if(millis() - lastDebounceGreen > debounceDelayGreen){
      ledGreenState = !ledGreenState;
      lastDebounceGreen = millis();
    }
}

void ElevatorElevatingSound(){
  tone(buzzer,500);
}
void ElevatorArrivesSound(){
  tone(buzzer,1000,300);
}