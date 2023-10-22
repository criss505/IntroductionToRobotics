const int greenInput = A0;
const int redInput = A1;
const int blueInput = A2;
const int ledPinGreen = 6;
const int ledPinRed = 5;
const int ledPinBlue = 3;
int greenValue = 0;
int redValue = 0;
int blueValue = 0;
const int analogHigh = 1023;
const int digitalHigh = 255;
 
void setup() {
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);
}

void loop() {
  // read pins input
  greenValue = analogRead(greenInput);
  redValue = analogRead(redInput);
  blueValue = analogRead(blueInput);

  // map values
  greenValue = map(greenValue, 0, analogHigh, 0, digitalHigh);
  redValue = map(redValue, 0, analogHigh, 0, digitalHigh);
  blueValue = map(blueValue, 0, analogHigh, 0, digitalHigh);

  // light pins
  analogWrite(ledPinGreen, greenValue);
  analogWrite(ledPinRed, redValue);
  analogWrite(ledPinBlue, blueValue);

  // delay in between reads for stability
  delay(1);
}
 