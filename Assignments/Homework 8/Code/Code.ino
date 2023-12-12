#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
// Pin definitions for the LED matrix
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
 
// Pin definitions for the joystick
const int xPin = A0;
const int yPin = A1;
const int swPin = A2;
 
// Thresholds and directions for joystick movement detection
const int minThreshold = 400;
const int maxThreshold = 600;
unsigned int readJoyStickDebounce = 300;
unsigned int lastReadJoystick = 0;
enum SwitchPosition {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  CENTER,
  NONE
};
 
// Timing variables for movement updates
int moveInterval = 200;  // tune this parameter for player speed
unsigned long lastMoved = 0;
 
// LedControl object for matrix
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); // DIN, CLK, LOAD, number of devices
 
// Size of the LED matrix (8x8)
const byte matrixSize = 8;
bool matrixChanged = true;

// 2D array representing the state (on/off) of each LED in the matrix
byte matrix[matrixSize][matrixSize] = { /* Initial state with all LEDs off */ };

// Variable to control LED matrix brightness
byte brightness[2] = {2,2}; // 0 - lcd brightness, 1 - matrix brightness
int brightnessEepromAddress = 0;
 

// --------------------------------------------------------------------
// lcd variables

const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// --------------------------------------------------------------------


// Menu variables
int gameState = 1; // 0 while playing, others while viewing menu
int selectedItem = 0; // current item displayed on menu
int selectedMenu = 0; // 0 for main menu, rest are submenus
int infoDelayTime = 0;
int infoDelayInterval = 1500;

String mainMenuItems[] = {"Start!", "Settings", "About the game"};
int mainMenuCurrentItem = 0; // current item displayed on menu
String settingsMenuItems[] = {"Menu light", "Game light", "Go back"};
int settingsMenuCurrentItem = 0;


// --------------------------------------------------------------------

// Snake variables
int snakeSize = 3;
// Define a struct to represent a coordinate
struct Coordinate {
  int x;
  int y;
};
Coordinate snake[] = {{2, 2}, {1, 2}, {0, 2}}; // Create a vertical 3-piece snake at the left edge of the matrix
byte xLastPos = 2;
byte yLastPos = 2;

// Represent direction
struct Direction {
  int dx;
  int dy;
};
// Last movement direction (initialised to right)
Direction lastDirection = {1, 0};


// Fruit variables
Coordinate fruit;
bool fruitSpawned = false;
// unsigned int fruitSpawnInterval = 100;
unsigned int fruitBlinkInterval = 100;
unsigned int fruitLastBlink = 0;
bool fruitOn = false;
unsigned int fruitsEaten = 0;

unsigned int remainingLives = 3;



// --------------------------------------------------------------------


void setup() {
  Serial.begin(9600);

  pinMode(swPin, INPUT_PULLUP);
  lc.shutdown(0, false); // Disable power saving, turn on the display

  // Set the brightness levels
  brightness[0] = EEPROM.read(brightnessEepromAddress);
  brightness[1] = EEPROM.read(brightnessEepromAddress +1);
  
  lc.setIntensity(0, brightness[1]);
  
  startUp();
  lcd.clear();
  matrix[snake[0].x][snake[0].y] = 1; // Turn on the initial LED position
  generateFruit();
}

void loop() {
  inMenu(gameState);
  if (gameState == 0) {
    inGame();
  }
}

void startUp() {
  // Display intro message on the LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("SNAKE!");
  lcd.setCursor(3, 1);
  lcd.print("welcome :)");

  // Light up the LEDs on the matrix in a spiral from the center to the border
  int startRow = 0, endRow = matrixSize - 1;
  int startCol = 0, endCol = matrixSize - 1;

  while (startRow <= endRow && startCol <= endCol) {
    for (int i = startCol; i <= endCol; i++) {
      lc.setLed(0, startRow, i, true);
      delay(20);
    }
    startRow++;

    for (int i = startRow; i <= endRow; i++) {
      lc.setLed(0, i, endCol, true);
      delay(20);
    }
    endCol--;

    if (startRow <= endRow) {
      for (int i = endCol; i >= startCol; i--) {
        lc.setLed(0, endRow, i, true);
        delay(20);
      }
      endRow--;
    }

    if (startCol <= endCol) {
      for (int i = endRow; i >= startRow; i--) {
        lc.setLed(0, i, startCol, true);
        delay(20);
      }
      startCol++;
    }
  }
  delay(100);
}

// ---------------------------------------------------------------


void inMenu(int state) {
  switch (state) {
    case 0:
      Serial.println("0. in game\n");
      gameStats();
      break;
    case 1:
      Serial.println("1. Main Menu\n");
      mainMenu();
      break;
    case 2:
      Serial.println("2. Settings Menu\n");
      settingsMenu();
      break;
    case 3:
      Serial.println("3. About the game\n");
      infoPage();
      break;
    case 4:
      Serial.println("4. After the game\n");
      afterGame();
      break;
    default:
      Serial.println("Not an option\n");
      Serial.print("Tried ");
      Serial.println(state);
  }
}


void mainMenu() {
  // Display the currently selected item
  lcd.setCursor(0, 0);
  lcd.print(mainMenuItems[mainMenuCurrentItem]);

  // Read the joystick input
  SwitchPosition switchPosition = readSwitch();

  if (millis() - lastReadJoystick > readJoyStickDebounce){
    lastReadJoystick = millis(); 
    if (switchPosition == DOWN) {
      mainMenuCurrentItem--;
      if (mainMenuCurrentItem < 0){
        mainMenuCurrentItem = 2;
      }
    } else if (switchPosition == UP) {
      mainMenuCurrentItem = (mainMenuCurrentItem+1) % 3;
    } else if (switchPosition == CENTER) {
      mainMenuChoice(mainMenuCurrentItem);
    }
    // Serial.println(mainMenuCurrentItem);

    // Update the display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(mainMenuItems[mainMenuCurrentItem]);
  }
}

void mainMenuChoice(int option){
  switch (option){
    case 0:
      // start game
      gameState = 0;
      lcd.clear();
      remainingLives = 3;
      fruitsEaten = 0;
      break;
    case 1:
      // settings menu
      gameState = 2;
      break;
    case 2:
      // about game
      gameState = 3;
      break;
    default:
      Serial.println("Not an option\n");
  }
}

void settingsMenu() {
  // Display the currently selected item
  lcd.setCursor(0, 0);
  lcd.print(settingsMenuItems[settingsMenuCurrentItem]);

  // Read the joystick input
  SwitchPosition switchPosition = readSwitch();

  if (millis() - lastReadJoystick > readJoyStickDebounce){
    lastReadJoystick = millis(); 
    if (switchPosition == DOWN) {
      settingsMenuCurrentItem--;
      if (settingsMenuCurrentItem < 0){
        settingsMenuCurrentItem = 2;
      }
    } 
    else if (switchPosition == UP) {
      settingsMenuCurrentItem = (settingsMenuCurrentItem+1) % 3;
    } 
    else if (switchPosition == LEFT && brightness[settingsMenuCurrentItem] > 0) {
      brightness[settingsMenuCurrentItem]--;
    }
    else if (switchPosition == RIGHT && brightness[settingsMenuCurrentItem] < 5) {
      brightness[settingsMenuCurrentItem] = brightness[settingsMenuCurrentItem] + 1;
    }
    else if (switchPosition == CENTER && settingsMenuCurrentItem == 2) {
      gameState = 1;
    }

    // Update the display
    lcd.clear();
    lcd.setCursor(0, 0);

    lcd.print(settingsMenuItems[settingsMenuCurrentItem]);
    lcd.setCursor(0, 1);

    if (settingsMenuCurrentItem < 2){ // show brightness levels
      lcd.print("Intensity: ");
      lcd.print(brightness[settingsMenuCurrentItem]);
      brightnessToEeprom();
    }

  }
}

void gameStats(){
  // display eaten fruits
  lcd.setCursor(0, 0);
  lcd.print("fruits: ");
  lcd.print(fruitsEaten);
  // display current lives
  lcd.setCursor(0, 1);
  lcd.print("lives: ");
  lcd.print(remainingLives);
}

void  infoPage(){
    lcd.clear();
  int scrollDebounce = millis() - infoDelayTime;
  if (scrollDebounce < infoDelayInterval) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Creator: ");
    lcd.setCursor(0, 1);
    lcd.print(" Cristina Damov ");
  }
  else if(scrollDebounce >= infoDelayInterval && scrollDebounce < infoDelayInterval * 2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Github: ");
    lcd.setCursor(0, 1);
    lcd.print(" criss505 ");
  }
  else if (scrollDebounce >= infoDelayInterval * 2 && scrollDebounce < infoDelayInterval * 3) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Game: ");
    lcd.setCursor(0, 1);
    lcd.print(" snake! ");
  } else {
    infoDelayTime = millis();
  }

  // Go back to main menu when doing any joystick movement
  SwitchPosition joystickReading = readSwitch();
  if (joystickReading != NONE) {
    gameState = 1;
  }

}

void afterGame(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("you died! xp");
  lcd.setCursor(0, 1);
  lcd.print("total fruits: ");
  lcd.print(fruitsEaten);

  // go back to main menu when doing any joystick movement
  SwitchPosition joystickReading = readSwitch();
  if (joystickReading != NONE) {
    gameState = 1;
  }

}



// ---------------------------------------------------------------

// Update new brightness variables in eeprom
void brightnessToEeprom(){ 
  EEPROM.update(brightnessEepromAddress, brightness[0]);
  EEPROM.update(brightnessEepromAddress +1, brightness[1]);
}

// Reads joystick state
SwitchPosition readSwitch(){
  // Read X-axis and Y-axis values
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int swValue = digitalRead(swPin);

  if (xValue < minThreshold) return DOWN;
  if (xValue > maxThreshold) return UP;
  if (yValue < minThreshold) return LEFT;
  if (yValue > maxThreshold) return RIGHT;
  if (swValue == LOW) return CENTER;
  return NONE;
}

 
// ---------------------------------------------------------------

void inGame(){
  // Display snake body parts
  for (int i = 0; i < snakeSize; i++) {
    lc.setLed(0, snake[i].x, snake[i].y, true);
  }

  // Check if it's time to move the LED
  if (millis() - lastMoved > moveInterval) {
    updatePositions(); // Update the LED position based on joystick input
    lastMoved = millis(); // Reset the movement timer
  }
 
  updateMatrix();
  checkMatrixBounds();

  generateFruit();
  blinkFruit();
  checkEatFruit();
}
 

// Update the LED matrix display
void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}
 
// Read joystick input and update LED positions accordingly
void updatePositions() {
  // Store the last positions
  xLastPos = snake[0].x;
  yLastPos = snake[0].y;

  // Shift snake
  for (int i = snakeSize-1; i > 0; i--) {
    snake[i] = snake[i - 1];
  }

  SwitchPosition joystickPosition = readSwitch();
  bool changeDirection = false;

  // Update xPos based on joystick movement
  if (lastDirection.dy != 0){ // Doesn't make sense to change direction to the face or back of the snake, only left/right
    if (joystickPosition == DOWN) {
      snake[0].x = snake[0].x + 1;
      lastDirection = {1, 0};
      changeDirection = true;
    } else if (joystickPosition == UP) {
      snake[0].x = snake[0].x - 1;
      lastDirection = {-1, 0};
      changeDirection = true;
    } 
    checkMatrixBounds();
  }

  // Update yPos based on joystick movement
  if (lastDirection.dx != 0){ // && snake[1].x == snake[0].x) {
    if (joystickPosition == LEFT && lastDirection.dy != 1) {
      snake[0].y = snake[0].y + 1;
      lastDirection = {0, 1};
      changeDirection = true;
    } else if (joystickPosition == RIGHT && lastDirection.dy != -1) {
      snake[0].y = snake[0].y - 1;
      lastDirection = {0, -1};
      changeDirection = true;
      checkMatrixBounds();
    }
  }

  if (changeDirection != true) {
    snake[0].x = snake[0].x + lastDirection.dx;
    snake[0].y = snake[0].y + lastDirection.dy;
  }
   
  checkMatrixBounds();
 

  // Check if the position has changed and update the matrix accordingly
  if (snake[0].x != xLastPos || snake[0].y != yLastPos) {
    matrixChanged = true;
    matrix[xLastPos][yLastPos] = 0; // Turn off the LED at the last position
    matrix[snake[0].x][snake[0].y] = 1; // Turn on the LED at the new position
  }
}
 
// Check if player encounters walls
void checkMatrixBounds() {
  if (snake[0].x < 0 || snake[0].x >= matrixSize || snake[0].y < 0 || snake[0].y >= matrixSize) { // Snake hit a wall
    remainingLives--;
    if (remainingLives == 0){
      gameState = 4;
    }

    snake[0].x = (snake[0].x + matrixSize) % matrixSize;  // Put snake back on map for next life
    snake[0].y = (snake[0].y + matrixSize) % matrixSize;
  }
}

// Random fruit generator
void generateFruit() {
  bool touchSnake; // The fruit can't generate in a position occupied by snake

  if (fruitSpawned == false) {  // Only one fruit will be generated at a time
    do {
      touchSnake = false;

      // Generate a random position within the matrix
      fruit.x = rand() % matrixSize;
      fruit.y = rand() % matrixSize;

      // Check if the position coincides with any part of the snake
      for (int i = 0; i < 3; i++) {
        if (snake[i].x == fruit.x && snake[i].y == fruit.y) {
          touchSnake = true;
          break;
        }
      }
      fruitSpawned = true; // fruit generated succesfully!
      Serial.println("Fruit generated");
    } while (touchSnake);
  }
}

void blinkFruit(){
  if (fruitSpawned) {
    if (millis() - fruitLastBlink >= fruitBlinkInterval) {
      fruitOn = !fruitOn; // Toggle the fruit's LED
      matrix[fruit.x][fruit.y] = fruitOn ? 1 : 0; // Update the matrix
      fruitLastBlink = millis(); // Update the last blink time
    }
  }
}

// Check if player encounters fruit
void checkEatFruit(){
  if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
    Serial.println("Fruit eaten");
    fruitsEaten++;
    fruitSpawned = false;
    generateFruit();

    // if (fruitsEaten % 3 == 1) { // for every 3 fruits eaten, snake size and speed is raised
    //   growSnake();
    //   moveInterval -= 10;
      
    //   Serial.print("Snake size: ");
    //   Serial.println(snakeSize);
    // }
  }
}

// Grow size of player
void growSnake() {
  snakeSize++;
  // Initialize the new tail segment based on the last movement direction
  snake[snakeSize - 1].x = snake[snakeSize - 2].x - lastDirection.dx;
  snake[snakeSize - 1].y = snake[snakeSize - 2].y - lastDirection.dy;
}