// Arduino code for the autonomous solar panel cleaning robot

/*
  Project: Autonomous Solar Panel Cleaning Robot
  Source: Reconstructed from the project report, Appendix A.

  IMPORTANT:
  - Verify motor wiring and movement directions before powering.
  - Install the NewPing library in Arduino IDE.
  - This reconstruction has not been hardware-tested.
*/

#include <Servo.h>
#include <NewPing.h>

// L298N motor driver pins (as listed in the report)
const int LeftMotorForward = 8;
const int LeftMotorBackward = 9;
const int RightMotorForward = 10;
const int RightMotorBackward = 11;

// HC-SR04 ultrasonic sensor pins
#define trig_pin 4
#define echo_pin 5
#define maximum_distance 200

boolean f1 = false, f2 = false, f3 = false;
boolean goesForward = false;
int stop1 = 0;
int distance = 100;

NewPing sonar(trig_pin, echo_pin, maximum_distance);
Servo servo_motor;

int readPing();
int lookRight();
int lookLeft();
void moveStop();
void moveForward();
void moveBackward();
void turnRight();
void turnLeft();

void setup() {
  Serial.begin(9600);

  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);

  // Preserve the initialization shown in the report.
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);

  delay(3000);

  servo_motor.attach(6);
  servo_motor.write(115);
  delay(2000);

  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {
  Serial.println(distance);
  distance = readPing();

  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

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

  if (distance <= distanceLeft && stop1 == 1) {
    turnRight();
    delay(300);
    moveStop();
    delay(300);
    moveForward();
    delay(300);
  }

  /*
    The report contains a second similar condition that
    turns left. It is retained here as reported, but its
    logic should be reviewed before physical testing.
  */
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
    moveForward();
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == 'A') {
      stop1 = 1;
      Serial.print("forward");
      moveForward();
    } else if (inChar == 'D') {
      Serial.print("reverse");
      moveBackward();
    } else if (inChar == 'R') {
      Serial.print("right");
      turnRight();
    } else if (inChar == 'L') {
      Serial.print("left");
      turnLeft();
    } else if (inChar == 'B') {
      stop1 = 0;
      Serial.print("stop");
      moveStop();
    }
  }
}

int lookRight() {
  servo_motor.write(50);
  delay(500);

  int measuredDistance = readPing();
  delay(100);

  servo_motor.write(115);
  return measuredDistance;
}

int lookLeft() {
  servo_motor.write(170);
  delay(500);

  int measuredDistance = readPing();
  delay(100);

  servo_motor.write(115);
  return measuredDistance;
}

int readPing() {
  delay(70);

  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }

  return cm;
}

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
