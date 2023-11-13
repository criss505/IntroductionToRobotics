// buttons
int button1 = A0;   // start/pause
int button2 = A1;   // reset time/saved laps
int button3 = A2;   // save/view laps
int buttonState1 = LOW; // state of each button
int buttonState2 = LOW;
int buttonState3 = LOW;
byte reading1 = LOW;  // reading of each button
byte reading2 = LOW;
byte reading3 = LOW;
byte lastReading1 = LOW; 
byte lastReading2 = LOW;
byte lastReading3 = LOW;
unsigned int lastDebounce1 = 0; // last debounce time for each button
unsigned int lastDebounce2 = 0;
unsigned int lastDebounce3 = 0;
unsigned int debounceDelay = 50;  // debounce time margin for button readings

// shift register
const int latchPin = 3; // STCP
const int clockPin = 2; // SHCP
const int dataPin = 4; // DS
 
// digit control pins for 4-digit display
const int segD1 = 6;
const int segD2 = 7;
const int segD3 = 8;
const int segD4 = 9;

// extra pins
const int buzzerPin = 11;
const int ledPin = 12;

// store digits in an array for easy access
const int displayCount = 4; // number of digits in the display
int displayDigits[] = {segD1, segD2, segD3, segD4};
 
// number encodings (0-9)
byte encodedDigitToWrite;
const int encodingsNumber = 10;
byte byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
};

// display control
unsigned long lastIncrement = 0;
unsigned long delayCount = 100; // delay between time updates
unsigned long number = 0; // displayed count number

byte isPaused = LOW;
byte isViewingLaps = LOW;

int savedLapsNumber = 4;
unsigned long savedLaps[] = {0, 0, 0, 0};
int currentViewedLap = 0;

// extra functionalities
int buzzerTone = 1000;
 
 
void setup() {
  // initialize buttons
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);

  // initialize pins in shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
 
  // initialize digit control pins to LOW (off)
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  // initialize led pin
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}
 
void loop() {
  ReadButtons();
  SetLedState();

  if (!isViewingLaps){  // counting mode
    NextNumber();
    WriteNumber(number); 
  }
  else{                 // viewing laps mode
    ShowCurrentSavedLap();
  }
} 

//increases timer count 
void NextNumber(){
  if (!isPaused){
    if (millis() - lastIncrement > delayCount) {
      number++;
      number = number % int(pow(10, displayCount));
      lastIncrement = millis();
    }
  }
}
 
void ActivateDisplay(int displayNumber) {
  // turn off pins to avoid ghosting
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
}

void WriteReg(int digit) {
    // prepare to shift data
    digitalWrite(latchPin, LOW);
    // set current digit to the shift register
    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    // latch the data onto the output pins
    digitalWrite(latchPin, HIGH);
}

void WriteNumber(int displayNumber) {
  int currentNumber = displayNumber;

  for (int displayDigit = displayCount-1; displayDigit >= 0; displayDigit--){
  	ActivateDisplay(displayDigit);
    encodedDigitToWrite = byteEncodings[currentNumber % 10];

    // add decimal point to the second digit
    if (displayDigit == displayCount-2){
      encodedDigitToWrite |= B00000001;
    }

    WriteReg(encodedDigitToWrite);
    currentNumber = currentNumber/10;
    WriteReg(B00000000);
  }
}

void ReadButtons(){
  // read button 3 - saved laps
  reading3 = digitalRead(button3);
  if (reading3 != lastReading3) {
    lastDebounce3 = millis();
  }
  if ((millis() - lastDebounce3) > debounceDelay) {
    if (buttonState3 != reading3){
      buttonState3 = reading3;
      if (buttonState3 == HIGH){
        LapsFunctionality();
      }
    }
  }
  lastReading3 = reading3;

  // read button 1 - pause/resume
  reading1 = digitalRead(button1);
  if (reading1 != lastReading1) {
    lastDebounce1 = millis();
  }
  if ((millis() - lastDebounce1) > debounceDelay) {
    if (buttonState1 != reading1){
      buttonState1 = reading1;
      if (buttonState1 == HIGH){
        PauseFunctionality();
      }
    }
  }
  lastReading1 = reading1;

  // read button 2 - resets
  reading2 = digitalRead(button2);
  if (reading2 != lastReading2) {
    lastDebounce2 = millis();
  }
  if ((millis() - lastDebounce2) > debounceDelay) {
    if (buttonState2 != reading2){
      buttonState2 = reading2;
      if (buttonState2 == HIGH){
        ResetFunctionality();
      }
    }
  }
  lastReading2 = reading2;
}

void PauseFunctionality(){
  if (!isViewingLaps){   // pauses or resumes while in counting mode
    ChangePauseResume();
    PrintTimeInConsole();
    Serial.println(": Pressed pause/resume button");
  }
}

void ResetFunctionality(){
  if (isViewingLaps){ // reset saved laps and exit lap viewing mode -> go back to start state
    ResetLaps();
    number = 0;
    ChangeLapViewingMode();
    isPaused = HIGH;
    SoundBuzzer();
    Serial.println("Exited lap viewing mode");
  }
  else if (isPaused){  // resets time to 0, can enter lap viewing mode if button 2 is pressed
    number = 0;
    SoundBuzzer();
    PrintTimeInConsole();
    Serial.println(": Reset timer to 0");
  }
}

void LapsFunctionality(){
  if (!isPaused){ // save lap while in counting mode
    SaveLap();
    PrintTimeInConsole();
    Serial.println(": Saved lap");
    // printLaps();
  }
  else{ // the button's functionalities related to viewing laps mode
    if (number == 0 && savedLaps[0] != 0 && isViewingLaps == LOW){  // if timer is reset and there are saved laps, enter the viewing laps mode
      Serial.println("LAP VIEWING MODE");
      ChangeLapViewingMode();
    }
    else if (isViewingLaps == HIGH){  // go to next saved laps while in viewing laps mode
      NextSavedLap();
    }
    // printLaps();
  }
}

void PrintTimeInConsole(){
    Serial.print(number / 10);
    Serial.print(",");
    Serial.print(number % 10);
}

void ChangePauseResume(){
  isPaused = !isPaused;
  SoundBuzzer();
}

void SaveLap(){
  if (!isPaused){
    for (int i=savedLapsNumber-1; i>0; i--){
      savedLaps[i] = savedLaps[i-1];
    }
    savedLaps[0] = number;
  }
  // doesn't save lap while paused
}

void ResetLaps(){
  for (int i=0; i<savedLapsNumber; i++){
    savedLaps[i] = 0;
  }
}

void ChangeLapViewingMode(){
  isViewingLaps = !isViewingLaps;
  currentViewedLap = 0;
}

// iterating laps while in viewing laps mode
void NextSavedLap(){
  currentViewedLap++;

  // won't consider empty laps
  if (savedLaps[currentViewedLap] == 0 || currentViewedLap >= savedLapsNumber){
    currentViewedLap = 0;
  }
}

// display the current saved lap while in viewing laps mode
void ShowCurrentSavedLap(){
  WriteNumber(savedLaps[currentViewedLap]);
  // Serial.print("Currently showing saved lap - ");
  // Serial.println(savedLaps[currentViewedLap]);
}

// prints currently saved laps in console - used mainly for debugging
void PrintLaps(){
  Serial.println("Currently saved laps:");
  for (int i=0; i<savedLapsNumber; i++){
    if (savedLaps[i]){
      Serial.println(savedLaps[i]);
    }
  }
}

void SoundBuzzer(){
  tone(buzzerPin, buzzerTone, 50);
}

void SetLedState(){
  if (!isPaused){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
}