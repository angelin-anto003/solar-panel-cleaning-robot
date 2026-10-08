```cpp
/*
 * Project: Autonomous Solar Panel Cleaning Robot
 * Description:
 * Arduino control for robot movement, ultrasonic distance
 * measurement, servo scanning, and Bluetooth serial commands.
 *
 * Source: Transcribed and structured from the college project report.
 * Hardware behavior has not been independently retested.
 */

#include <Servo.h>
#include <NewPing.h>

// =====================================================
// 1. MOTOR DRIVER PIN CONFIGURATION (L298N)
// =====================================================

const int LeftMotorForward  = 8;
const int LeftMotorBackward = 9;
const int RightMotorForward = 10;
const int RightMotorBackward = 11;

// =====================================================
// 2. ULTRASONIC SENSOR CONFIGURATION (HC-SR04)
// =====================================================

#define trig_pin 4
#define echo_pin 5
#define maximum_distance 200

NewPing sonar(trig_pin, echo_pin, maximum_distance);

// =====================================================
// 3. SERVO MOTOR CONFIGURATION
// =====================================================

Servo servo_motor;
const int servo_pin = 6;

// =====================================================
// 4. GLOBAL VARIABLES
// =====================================================

boolean goesForward = false;

int stop1 = 0;
int distance = 100;

boolean f1 = false;
boolean f2 = false;
boolean f3 = false;

// =====================================================
// 5. SETUP
// =====================================================

void setup() {
  Serial.begin(9600);

  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);

  // Initial motor output state from the report
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);

  delay(3000);

  servo_motor.attach(servo_pin);
  servo_motor.write(115);

  delay(2000);

  // Initialize distance readings
  for (int i = 0; i < 4; i++) {
    distance = readPing();
    delay(100);
  }
}

// =====================================================
// 6. MAIN ROBOT LOOP
// =====================================================

void loop() {
  Serial.println(distance);

  distance = readPing();

  int distanceRight = 0;
  int distanceLeft = 0;

  delay(50);

  /*
   * Obstacle-avoidance logic in the supplied report
   * contains unclear and duplicated code blocks.
   * Review against the original working sketch before
   * relying on this section for autonomous operation.
   */

  if (distance >= 20 && goesForward == true && stop1 == 1) {
    Serial.println("function111");

    moveStop();
    delay(300);

    distanceRight = lookRight();
    delay(300);

    distanceLeft = lookLeft();
    delay(300);

    moveBackward();
    delay(500);

    moveStop();
    delay(300);

    turnLeft();
    delay(300);

    moveStop();
    delay(300);

    Serial.println("function111 stop");
  }

  // Second obstacle-avoidance sequence from the report
  f1 = false;

  moveStop();
  delay(300);

  distanceRight = lookRight();
  delay(300);

  distanceLeft = lookLeft();
  delay(300);

  moveBackward();
  delay(800);

  moveStop();
  delay(300);

  turnRight();
  delay(10000);

  moveStop();

  Serial.println("function222 stop");

  distance = readPing();

  if (distance <= distanceLeft && stop1 == 1) {
    turnRight();
    delay(300);

    moveStop();
    delay(300);

    moveForward();
    delay(300);
  }

  if (distance <= distanceLeft && stop1 == 1) {
    turnLeft();
    delay(300);

    moveStop();
    delay(300);

    moveForward();
    delay(300);
  }

  if (distance < 20 && goesForward == true && stop1 == 1) {
    Serial.println("forward");
  }
}

// =====================================================
// 7. BLUETOOTH SERIAL COMMAND HANDLING
// =====================================================

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    switch (inChar) {
      case 'A':
        stop1 = 1;
        Serial.print("forward");
        moveForward();
        break;

      case 'D':
        Serial.print("reverse");
        moveBackward();
        break;

      case 'R':
        Serial.print("right");
        turnRight();
        break;

      case 'L':
        Serial.print("left");
        turnLeft();
        break;

      case 'B':
        stop1 = 0;
        Serial.print("stop");
        moveStop();
        break;
    }
  }
}

// =====================================================
// 8. SERVO SCANNING AND DISTANCE MEASUREMENT
// =====================================================

int lookRight() {
  servo_motor.write(50);
  delay(500);

  int distance = readPing();

  delay(100);
  servo_motor.write(115);

  return distance;
}

int lookLeft() {
  servo_motor.write(170);
  delay(500);

  int distance = readPing();

  delay(100);
  servo_motor.write(115);

  return distance;
}

int readPing() {
  delay(70);

  int cm = sonar.ping_cm();

  if (cm == 0) {
    cm = 250;
  }

  return cm;
}

// =====================================================
// 9. MOTOR CONTROL FUNCTIONS
// =====================================================

void moveStop() {
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveForward() {
  goesForward = true;

  Serial.println("forward");

  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, HIGH);

  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);
}

void moveBackward() {
  Serial.println("reverse");

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);

  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, HIGH);
}

void turnRight() {
  Serial.println("right");

  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, HIGH);

  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, HIGH);
}

void turnLeft() {
  Serial.println("left");

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);

  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);
}
```
