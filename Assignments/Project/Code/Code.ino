#include <QTRSensors.h>

const int m11Pin = 7;
const int m12Pin = 6;
const int m21Pin = 5;
const int m22Pin = 4;
const int m1Enable = 11;
const int m2Enable = 10;

int m1Speed = 0;
int m2Speed = 0;

float kp = 15;
float ki = 0;
float kd = 7;
int p = 1;
int i = 0;
int d = 0;

int error = 0;
int lastError = 0;

const int maxSpeed = 255;
const int minSpeed = -255;
const int baseSpeed = 255;

QTRSensors qtr;
const int sensorCount = 6;
int sensorValues[sensorCount];
int sensors[sensorCount] = {0, 0, 0, 0, 0, 0};

bool inInput = false;
int currInput = 0;

unsigned long long debugInterval = 0;
int debugDebounce = 500;


void setup() {
  // pinMode setup
  pinMode(m11Pin, OUTPUT);
  pinMode(m12Pin, OUTPUT);
  pinMode(m21Pin, OUTPUT);
  pinMode(m22Pin, OUTPUT);
  pinMode(m1Enable, OUTPUT);
  pinMode(m2Enable, OUTPUT);

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5}, sensorCount);
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); 

  for (uint16_t i = 0; i < 200; i++)
  {
    // 40 calibrate calls = 1 second
    if((i / 20) % 2 == 0)
      setMotorSpeed(0, 250); ///250
    if((i / 20) % 2 == 1)
      setMotorSpeed(0, -255); /// 255
    qtr.calibrate(); 
  }
  setMotorSpeed(0, 0);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
}


void loop() {
  if(inInput == false) pidControl(kp, ki, kd);

  if(Serial.available()) {
    float input = Serial.parseFloat();
    if(inInput == false) {
      inInput = true;
      setMotorSpeed(0, 0);
    }
    else{
      if(currInput == 0) {
        kp = input;
        currInput++;
      }
      else if(currInput == 1) ki = input, currInput++;
      else if(currInput == 2) {
        kd = input;
        currInput = 0;
        inInput = false;
      }
    }
  }

}

// calculate PID value based on error, kp, kd, ki, p, i and d.
void pidControl(float kp, float ki, float kd) {
  int error = map(qtr.readLineBlack(sensorValues), 0, 5000, -50, 50);

  int base = baseSpeed;
  p = error;
  i = i + error;
  d = error - lastError;
  int motorSpeed = 0;
  if(error <= 4 && error >= -4) error = 0;
  else if(error <= 13 && error >= -13) motorSpeed = 0.8 * p + ki * i + 5 * d;
  else if(error >= 42 || error <= -42) 
  {
    motorSpeed = 23 * p + ki * i + (kd - 2) * d; //20.3
    base = 245;
  }
  else if(error >= 35 || error <= -35) {
    base = 245;
    motorSpeed = kp * p + ki * i + kd * d;
  }
  else motorSpeed = kp * p + ki * i + kd * d; // = error in this case
  
  m1Speed = base;
  m2Speed = base;

  if (error < 0) {
    m2Speed += motorSpeed;
  }
  else if (error > 0) {
    m1Speed -= motorSpeed;
  }
  
  m1Speed = constrain(m1Speed, minSpeed, maxSpeed);
  m2Speed = constrain(m2Speed, minSpeed, maxSpeed);
  setMotorSpeed(m1Speed, m2Speed);
  lastError = error;

  // DEBUGGING
  if(millis() - debugInterval > debugDebounce) {
    Serial.println();
    Serial.print("Kp Ki Kd: ");
    Serial.print(kp);
    Serial.print(" ");
    Serial.print(ki);
    Serial.print(" ");
    Serial.println(kd);

    
    Serial.print("P I D: ");
    Serial.print(p);
    Serial.print(" ");
    Serial.print(i);
    Serial.print(" ");
    Serial.println(d);

    Serial.print("Motorspeed: ");
    Serial.println(motorSpeed);
    Serial.print("M1 speed: ");
    Serial.println(m1Speed);
    
    Serial.print("M2 speed: ");
    Serial.println(m2Speed);
    debugInterval = millis();
    Serial.print("Sensors: ");
    for (uint8_t i = 0; i < 6; i++)
    {
      Serial.print(sensorValues[i]);
      Serial.print('\t');
    }
  Serial.println();
  }
}


void setMotorSpeed(int motor1Speed, int motor2Speed) {
  if (motor1Speed == 0) {
    digitalWrite(m11Pin, LOW);
    digitalWrite(m12Pin, LOW);
    analogWrite(m1Enable, motor1Speed);
  }
  else {
    if (motor1Speed > 0) {
      digitalWrite(m11Pin, HIGH);
      digitalWrite(m12Pin, LOW);
      analogWrite(m1Enable, motor1Speed);
    }
    if (motor1Speed < 0) {
      digitalWrite(m11Pin, LOW);
      digitalWrite(m12Pin, HIGH);
      analogWrite(m1Enable, -motor1Speed);
    }
  }

  if (motor2Speed == 0) {
    digitalWrite(m21Pin, LOW);
    digitalWrite(m22Pin, LOW);
    analogWrite(m2Enable, motor2Speed);
  }
  else {
    if (motor2Speed > 0) {
      digitalWrite(m21Pin, HIGH);
      digitalWrite(m22Pin, LOW);
      analogWrite(m2Enable, motor2Speed);
    }
    if (motor2Speed < 0) {
      digitalWrite(m21Pin, LOW);
      digitalWrite(m22Pin, HIGH);
      analogWrite(m2Enable, -motor2Speed);
    }
  }
}