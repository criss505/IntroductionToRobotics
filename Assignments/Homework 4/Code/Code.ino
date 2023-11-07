// joystick pins
const int pinSW = 2; // switch output
const int pinX = A0; // X axis output
const int pinY = A1; // Y axis output
 
// led segments pins
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int segSize = 8;
int index = 7;    // starting from the decimal point
 
// buzzer
const int buzzer = 13;
int buzzerTone = 1000;

// to modify in case of common anode
bool commonAnode = false;
byte state = HIGH;


int segments[segSize] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
                  // A,B,C,D,E,F,G, .
byte lightList[8] = {0,0,0,0,0,0,0,0};  // which segments are lit up
                  // 0,1,2,3,4,5,6,7
 
int nextMove[segSize][4] = // going UP/DOWN/LEFT/RIGHT
{
  {-1, 6, 5, 1},
  {0, 6, 5, -1},
  {6, 3, 4, 7},
  {6, -1, 4, 2},
  {6, 3, -1, 2},
  {0, 6, -1, 1},
  {0, 3, -1, -1},
  {-1, -1, 2, -1}
};

unsigned int debounceDelay = 50;  // for joystick basic readings
unsigned int debounceDelayReset = 500;  // for joystick long press
unsigned int debounceDelayBlink = 200;  // for current segment blink
unsigned int lastDebounceSwitch = 0;
unsigned int lastDebounceX = 0;
unsigned int lastDebounceY = 0;
unsigned int blinkTime = 0;
unsigned int totalTime = 0;
 
 // other joystick variables
byte switchState = LOW;
byte lastSwitchState = LOW;
int switchValue = 0;
int xValue = 0;
int yValue = 0;
int xLastValue = 0;
int yLastValue = 0;
int joystickThresholdLow = 400;   // the limit for considering the joystick as moving
int joystickThresholdHigh = 600;
byte xStateUp = LOW;
byte xStateDown = LOW;
byte yStateRight = LOW;
byte yStateLeft = LOW;


void setup() {
  // initialize segment pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  // activate pull-up resistor on the push-button pin
  pinMode(pinSW, INPUT_PULLUP);
  // if common anode
  if (commonAnode == true) {
  	state = !state;
  }
  Serial.begin(9600);
}
 
void loop() {
  ReadJoystick();
  DisplayCurrentSegment();
  BlinkCurrent();
}

void ReadJoystick(){
  switchValue = digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  CheckX();
  CheckY();
  CheckSwitch();
}
 
void CheckX()
{
  // check X UP
  if ((xValue > joystickThresholdHigh) ^ (xLastValue > joystickThresholdHigh)){
    lastDebounceX = millis();
  }
  if (millis() - lastDebounceX > debounceDelay){
    if ((xValue > joystickThresholdHigh) ^ (xStateUp == state)){
      xStateUp = xValue > joystickThresholdHigh;
      if (xStateUp){
        MoveUp();
        // Serial.println("UP");
      }
    }
  }
  xLastValue = xValue;
  
 // check X DOWN
  if ((xValue < joystickThresholdLow) ^ (xLastValue < joystickThresholdLow)){
    lastDebounceX = millis();
  }
  if (millis() - lastDebounceX > debounceDelay){
    if ((xValue < joystickThresholdLow) ^ (xStateDown == state)){
      xStateDown = xValue < joystickThresholdLow;
      if (xStateDown){
        MoveDown();
        // Serial.println("DOWN");
      }
    }
  }
  xLastValue = xValue;
}

void CheckY()
{
 // check Y RIGHT
  if ((yValue > joystickThresholdHigh) ^ (yLastValue > joystickThresholdHigh)){
    lastDebounceY = millis();
  }
  if (millis() - lastDebounceY > debounceDelay){
    if ((yValue > joystickThresholdHigh) ^ (yStateRight == state)){
      yStateRight = yValue > joystickThresholdHigh;
      if (yStateRight){
        MoveRight();
        // Serial.println("RIGHT");
      }
    }
  }
  yLastValue = yValue;
  
 // check Y LEFT
  if ((yValue < joystickThresholdLow) ^ (yLastValue < joystickThresholdLow)){
    lastDebounceY = millis();
  }
  if (millis() - lastDebounceY > debounceDelay){
    if ((yValue < joystickThresholdLow) ^ (yStateLeft == state)){
      yStateLeft = yValue < joystickThresholdLow;
      if (yStateLeft){
        MoveLeft();
        // Serial.println("LEFT");
      }
    }
  }
  yLastValue = yValue;
}

void CheckSwitch()
{
  if (switchValue != lastSwitchState) {
    lastDebounceSwitch = millis();
  }

  if ((millis() - lastDebounceSwitch) > debounceDelay) {
    if (switchValue != switchState) {
      switchState = switchValue;
      if (switchState == state) { 
        // Serial.println(totalTime);
        if (totalTime > debounceDelayReset)
          ResetDisplay();
        else if (totalTime > debounceDelay)
          PushButton();
      }
    }
    else if (switchState == !state) {
      totalTime = millis()-lastDebounceSwitch;
    }
  }
  lastSwitchState = switchValue;
}

void DisplayCurrentSegment() {
  for (int i = 0; i < segSize; ++i) {
  	digitalWrite(segments[i],lightList[i]);
  }
  digitalWrite(segments[index], state);
}

void MoveUp(){
  if (nextMove[index][0] != -1){
    index = nextMove[index][0];
  }
  else{
    BuzzerSound();
  }
}

void MoveDown(){
  if (nextMove[index][1] != -1){
    index = nextMove[index][1];
  }
  else{
    BuzzerSound();
  }
}

void MoveLeft(){
  if (nextMove[index][2] != -1){
    index = nextMove[index][2];
  }
  else{
    BuzzerSound();
  }
}

void MoveRight(){
  if (nextMove[index][3] != -1){
    index = nextMove[index][3];
  }
  else{
    BuzzerSound();
  }
}

void PushButton(){
  lightList[index] = !lightList[index];
  // Serial.println(index);
}

void BlinkCurrent(){
  blinkTime = (millis() / debounceDelayBlink) % 2;
  digitalWrite(segments[index], blinkTime);
}

void ResetDisplay(){
  for (int i = 0; i < segSize; ++i)
  	lightList[i] = 0;
}

void BuzzerSound(){
    tone(buzzer, buzzerTone, 50);
}
