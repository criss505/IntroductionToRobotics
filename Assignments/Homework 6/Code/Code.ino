// photocell
int photocellPin = A0;   // the cell and 10K pulldown are connected to a0
int photocellValue;      // the analog reading from the sensor divider
int photocellThreshold;  // lower limit - signals it's too dark
int photocellEepromStart = 0;

// ultrasonic
int trigPin = 4;
int echoPin = 5;
int ultrasonicValue;
int ultrasonicThreshold;  // lower limit - signals an object is too close
int ultrasonicEepromStart = 10;
float distanceFix = 0.017;

// RGB
const int ledPinRed = 9;
const int ledPinGreen = 10;
const int ledPinBlue = 11;
int greenValue = 500;
int redValue = 500;
int blueValue = 0;
byte autoMode = LOW;

// button
int buttonPin = 3;
int buttonState = LOW;
byte reading = LOW;  // button reading value
byte lastReading = LOW;
unsigned int lastDebounce = 0;
unsigned int debounceDelay = 0;

// control menu
int currentMenuState = 0;
byte checkingMenu = HIGH;
byte readingSensors = LOW;

// others
int sampleDebounceTime;
unsigned long lastSampleDebounceTime = 0;
int loggedDataCount = 10;
int loggedSampleAddress = 20;
int loggedUltrasonicThresholdAddress = 21;
int loggedPhotocellThresholdAddress = 22;


#include <EEPROM.h>
void setup() {
  // Initialize button
  pinMode(buttonPin, INPUT_PULLUP);
  // Initialize ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Initialize RGB
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);

  // Initialise sensor settings
  sampleDebounceTime = EEPROM.read(loggedSampleAddress);
  photocellThreshold = EEPROM.read(loggedPhotocellThresholdAddress);
  ultrasonicThreshold = EEPROM.read(loggedUltrasonicThresholdAddress);

  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  // Display the initial menu interface to the Serial Monitor upon startup
  // printMenu();
}

void loop() {
  // Continuously checks for incoming serial data
  if (checkingMenu) {
    if (Serial.available()) {
      // Reads an integer value from the serial buffer (user's menu choice)
      int choice = Serial.parseInt();

      // Check if a valid integer was read
      if (Serial.read() == '\n') {
        // Calls function to print a message based on the user's choice
        printOptionMessage(choice);
        // Re-displays the menu after executing the chosen action
        printMenu();
      }
    }
  }
  if (millis() - lastSampleDebounceTime >= sampleDebounceTime) {
    lastSampleDebounceTime = millis();
    photocellSensorReading();
    ultrasonicSensorReading();
    addLoggedData();
  }
  checkAlert();
  readButton();
}






// menu functions
void printMenu() {
  switch (currentMenuState) {
    case 0:
      mainMenu();
      break;
    case 1:
      sensorMenu();
      break;
    case 2:
      resetMenu();
      break;
    case 3:
      statusMenu();
      break;
    case 4:
      ledMenu();
      break;
    default:
      Serial.println("Wrong current menu status\n");
  }
}

// choose current menu
void printOptionMessage(int choice) {  
  switch (currentMenuState) {
    case 0:
      mainMenuOption(choice);
      break;
    case 1:
      sensorMenuOption(choice);
      break;
    case 2:
      resetMenuOption(choice);
      break;
    case 3:
      statusMenuOption(choice);
      break;
    case 4:
      ledMenuOption(choice);
      break;
    default:
      Serial.println("Wrong current menu status\n");
  }
}

void mainMenu() {
  Serial.println("Menu Interface:");
  Serial.println("1. Set Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. Check System Status");
  Serial.println("4. Control LED");
}

void sensorMenu() {
  Serial.println("Sensor Menu:");
  Serial.println("1. Sensors Sampling Interval");
  Serial.println("2. Ultrasonic Alert Threshold");
  Serial.println("3. LDR Alert Threshold");
  Serial.println("4. Return to Main Menu");
}

void resetMenu() {
  Serial.println("Are you sure you want to reset logger data?");
  Serial.println("1. Yes");
  Serial.println("2. No");
}

void statusMenu() {
  Serial.println("System Status Menu:");
  Serial.println("1. Current Sensor Readings");
  Serial.println("2. Current Sensor Settings");
  Serial.println("3. Display Logged Data");
  Serial.println("4. Return to Main Menu");
}

void ledMenu() {
  Serial.println("LED Control Menu:");
  Serial.println("1. Manual Color Control");
  Serial.println("2. LED: Toggle Automatic ON/OFF");
  Serial.println("3. Return to Main Menu");
}

void mainMenuOption(int option) {
  Serial.print("\nPrinted message: ");
  switch (option) {
    case 1:
      Serial.println("1. Set Sensor Settings\n");
      currentMenuState = 1;
      break;
    case 2:
      Serial.println("2. Reset Logger Data\n");
      currentMenuState = 2;
      break;
    case 3:
      Serial.println("3. Check System Status\n");
      currentMenuState = 3;
      break;
    case 4:
      Serial.println("4. RGB LED Control\n");
      currentMenuState = 4;
      break;
    default:
      Serial.println("Not an option\n");
  }
}

void sensorMenuOption(int option) {
  Serial.print("\nPrinted message: ");
  switch (option) {
    case 1:
      Serial.println("1. Sensors Sampling Interval");
      setSampleDebounceTime();
      break;
    case 2:
      Serial.println("2. Ultrasonic Alert Threshold");
      setUltrasonicThreshold();
      break;
    case 3:
      Serial.println("3. LDR Alert Threshold");
      setPhotocellThreshold();
      break;
    case 4:
      Serial.println("4. Return to Main Menu");
      currentMenuState = 0;
      break;
    default:
      Serial.println("Not an option\n");
  }
}

void resetMenuOption(int option) {
  Serial.print("\nPrinted message: ");
  switch (option) {
    case 1:
      Serial.println("1. Yes\n");
      emptyLoggedData();
      break;
    case 2:
      Serial.println("2. No\n");
      break;
    default:
      Serial.println("Not an option\n");
  }
  currentMenuState = 0;  // go back to main menu, regardless of your choice of deleting logged data
}

void statusMenuOption(int option) {
  Serial.print("\nPrinted message: ");
  switch (option) {
    case 1:
      Serial.println("1. Current Sensor Readings\n");
      checkingMenu = LOW;
      readingSensors = HIGH;
      break;
    case 2:
      Serial.println("2. Current Sensor Settings\n");
      displayCurrentSensorSettings();
      break;
    case 3:
      Serial.println("3. Display Logged Data\n");
      displayLoggedData();
      break;
    case 4:
      Serial.println("4. Return to Main Menu\n");
      currentMenuState = 0;
      break;
    default:
      // Handles any choices outside the defined options
      Serial.println("Not an option\n");
  }
}

void ledMenuOption(int option) {
  Serial.print("\nPrinted message: ");
  switch (option) {
    case 1:
      Serial.println("1. Manual Color Control\n");
      setLed();
      lightLed();
      break;
    case 2:
      Serial.println("2. LED: Toggle Automatic ON/OFF\n");
      toggleLedMode();
      break;
    case 3:
      Serial.println("3. Return to Main Menu\n");
      currentMenuState = 0;
      break;
    default:
      Serial.println("Not an option\n");
  }
}


// other functionalities

void readButton() {
  reading = digitalRead(buttonPin);
  if (reading != lastReading) {
    lastDebounce = millis();
  }
  if ((millis() - lastDebounce) > debounceDelay) {
    if (buttonState != reading) {
      buttonState = reading;
      if (buttonState == HIGH) {
        stopSensors();
      }
    }
  }
  lastReading = reading;
}


// sensor readings
void photocellSensorReading() {
  photocellValue = analogRead(photocellPin);

  if (readingSensors) {
    Serial.print("Photocell reading = ");
    Serial.println(photocellValue);
  }
}

void ultrasonicSensorReading() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin => convert to distance
  ultrasonicValue = pulseIn(echoPin, HIGH) * distanceFix;

  // print value to Serial Monitor
  if (readingSensors) {
    Serial.print("Ultrasonic reading: ");
    Serial.println(ultrasonicValue);
    Serial.println("To exit sensor reading mode, please press the button.\n");
  }
}

void stopSensors() {
  checkingMenu = HIGH;
  readingSensors = LOW;
  printMenu();
}


// sensor settings functions
void setSampleDebounceTime() {
  Serial.println("Set the sampling rate with a value between 1 and 10 (seconds):");

  while (true) {
    if (Serial.available() > 0) {
      int inputValue = Serial.parseInt();

      if (inputValue >= 1 && inputValue <= 10) {
        sampleDebounceTime = inputValue * 1000;
        EEPROM.update(loggedSampleAddress, sampleDebounceTime);
        Serial.println("Value saved: " + String(sampleDebounceTime));
        break;
      } else {
        Serial.println("Invalid value! Please enter a value between 1 and 10.");
      }
    }
  }
}

void setUltrasonicThreshold() {
  Serial.println("Enter the ultrasonic threshold value (cm):");

  while (true) {
    if (Serial.available() > 0) {
      ultrasonicThreshold = Serial.parseInt();
      EEPROM.update(loggedUltrasonicThresholdAddress, ultrasonicThreshold);
      Serial.println("Threshold value saved: " + String(ultrasonicThreshold) + " cm");
      break;
    }
  }
}

void setPhotocellThreshold() {
  Serial.println("Enter the photocell threshold value (0-1023):");

  while (true) {
    if (Serial.available() > 0) {
      photocellThreshold = Serial.parseInt();
      EEPROM.update(loggedUltrasonicThresholdAddress, photocellThreshold);
      Serial.println("Threshold value saved: " + String(photocellThreshold));
      break;
    }
  }
}

void displayCurrentSensorSettings() {
  Serial.print("Current sampling time rate: ");
  Serial.print(sampleDebounceTime / 1000);
  Serial.println(" seconds");
  Serial.print("Current photocell low threshold: ");
  Serial.println(photocellThreshold);
  Serial.print("Current ultrasonic low threshold: ");
  Serial.print(ultrasonicThreshold);
  Serial.println(" cm");
  Serial.println("(When passing thresholds limits, the led will alert you)\n");
}


// logged data functions
void addLoggedData() {
  // move eeprom data one step
  for (int valueAddress = photocellEepromStart + loggedDataCount - 1; valueAddress > photocellEepromStart; valueAddress--) {
    int value = EEPROM.read(valueAddress - 1);
    EEPROM.update(valueAddress, value);
  }
  for (int valueAddress = ultrasonicEepromStart + loggedDataCount - 1; valueAddress > ultrasonicEepromStart; valueAddress--) {
    int value = EEPROM.read(valueAddress - 1);
    EEPROM.update(valueAddress, value);
  }
  // add current ultrasonicValue and photocellValue to eeprom
  EEPROM.update(photocellEepromStart, photocellValue);
  EEPROM.update(ultrasonicEepromStart, ultrasonicValue);
}

void emptyLoggedData() {
  for (int valueAddress = photocellEepromStart; valueAddress < photocellEepromStart + loggedDataCount; valueAddress++) {
    EEPROM.update(valueAddress, 0);
  }
  for (int valueAddress = ultrasonicEepromStart; valueAddress < ultrasonicEepromStart + loggedDataCount; valueAddress++) {
    EEPROM.update(valueAddress, 0);
  }
  Serial.println("Logged data was removed.");
}

void displayLoggedData() {
  Serial.print("Showing data for the last ");
  Serial.print(loggedDataCount);
  Serial.println(" readings:\n");

  for (int count = 0; count < loggedDataCount; count++) {
    photocellValue = EEPROM.read(count + photocellEepromStart);
    ultrasonicValue = EEPROM.read(count + ultrasonicEepromStart);
    Serial.print("photocell: ");
    Serial.println(photocellValue);
    Serial.print("ultrasonic: ");
    Serial.println(ultrasonicValue);
    Serial.print("\n");
  }
}


// led functions
void setLed() {
  Serial.println("Enter RGB values (0 to 255) for the LED:");

  Serial.println("Red: ");
  while (!Serial.available()) {}
  redValue = Serial.parseInt();

  Serial.println("Green: ");
  while (!Serial.available()) {}
  greenValue = Serial.parseInt();

  Serial.println("Blue: ");
  while (!Serial.available()) {}
  blueValue = Serial.parseInt();

  // print the entered values
  Serial.println("Entered RGB values:");
  Serial.print("Red: ");
  Serial.println(redValue);
  Serial.print("Green: ");
  Serial.println(greenValue);
  Serial.print("Blue: ");
  Serial.println(blueValue);

  autoMode = LOW;  // set automatic mode off in case it's on
}

void lightLed() {
  // light pins
  analogWrite(ledPinGreen, greenValue);
  analogWrite(ledPinRed, redValue);
  analogWrite(ledPinBlue, blueValue);

  // delay in between reads for stability
  delay(1);
}

void toggleLedMode() {
  autoMode = !autoMode;
}

void checkAlert() {
  if (autoMode) {
    if (photocellValue < photocellThreshold || ultrasonicValue < ultrasonicThreshold) {
      // red led
      analogWrite(ledPinGreen, 0);
      analogWrite(ledPinRed, 1000);
      analogWrite(ledPinBlue, 0);

    } else {
      // green led
      analogWrite(ledPinGreen, 1000);
      analogWrite(ledPinRed, 0);
      analogWrite(ledPinBlue, 0);
    }
  } else {
    lightLed();  // this function uses the manually inputed valued (redValue, greenValue, blueValue);
  }
}
