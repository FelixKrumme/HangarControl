#include <Arduino.h>
// #include "serial_communication.h"
#include "motor_control.h"

// Put here Interupt Service Routines for Endtriggers

// put function declarations here:
int myFunction(int, int);
StepperMotor motor_big_centring_left = StepperMotor(motor_group_big_centring, platform_front_left, 48, 46, 8, 2);
StepperMotor motor_big_centring_right = StepperMotor(motor_group_big_centring, platform_front_right, 52, 50, 8, 2);

StepperGroup group_big_centring = StepperGroup(1, 1000, HIGH);

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
  };
  Serial.println("Serial Communication started..");
  pinMode(46, OUTPUT); // Motor 1 Pul
  pinMode(48, OUTPUT); // Motor 1 Dir
  pinMode(50, OUTPUT); // Motor 2 Pul
  pinMode(52, OUTPUT); // Motor 2 Dir
  pinMode(23, INPUT);  // Switch for Big Centring
  group_big_centring.addMotor(&motor_big_centring_left);
  group_big_centring.addMotor(&motor_big_centring_right);
  // 10000 Steps 876mm
  // group_big_centring.moveGroupBySteps(10000, HIGH, 5000);
  // 400mm -> 4566(.2) Steps
  // group_big_centring.moveGroupBySteps(4566, LOW, 5000); // 293mm
  // Sweet Spot of Speed for big centring probably is somewhere between 2000 and 4000
  // group_big_centring.moveGroupBySteps(4566, HIGH, 3000);
  // group_big_centring.moveGroupBySteps(4566, LOW, 2000);
  // group_big_centring.moveGroupBySteps(4566, HIGH, 1000);

  // put your setup code here, to run once:
}

char state = 'p'; // Program state: 'r' for running, 'p' for paused

void loop()
{
  // Check if there is data available to read from the serial port
  if (Serial.available() > 0)
  {
    // Read the incoming byte
    char incomingByte = Serial.read();

    // If it's a 'p', pause the program
    if (incomingByte == 'p')
    {
      state = 'p';
      Serial.println("Pausing program");
    }
    else if (incomingByte == 'f')
    {
      state = 'f';
      Serial.println("Moving big centring forward");
    }

    else if (incomingByte == 'b')
    {
      state = 'b';
      Serial.println("Moving big centring back");
    }
    // If it's an 'r', resume the program
    else if (incomingByte == 'r')
    {
      state = 'r';
      Serial.println("Moving back and forth");
    }
  }

  // If the program is not paused, run the motor group
  if (state == 'p')
  {
    if (digitalRead(23) == HIGH)
    {
      group_big_centring.moveGroupBySteps(10, HIGH, 5000);
    }
    if (digitalRead(22) == HIGH)
    {
      group_big_centring.moveGroupBySteps(10, LOW, 5000);
    }
  }
  else if (state == 'f')
  {
    group_big_centring.moveGroupBySteps(10, HIGH, 3000);
  }
  else if (state == 'b')
  {
    group_big_centring.moveGroupBySteps(10, LOW, 4000);
  }
  else if (state == 'r')
  {
    group_big_centring.moveGroupBySteps(10, HIGH, 5000);
    group_big_centring.moveGroupBySteps(10, LOW, 5000);
  }
  if (digitalRead(23) == HIGH)
  {
    group_big_centring.moveGroupBySteps(10, HIGH, 5000);
  }

  // digitalWrite(12, HIGH);
  // stepper1.moveByStepsBlocking(400, 5000);
  // digitalWrite(12, LOW);
  // stepper1.moveByStepsBlocking(800, 4000);
  // delay(1000);

  // group1.moveGroupByRotations(10, HIGH);

  // group1.moveGroupByRotations(10, LOW);

  // digitalWrite(17, HIGH);
  // digitalWrite(25, HIGH);

  // for (unsigned int i = 0; i < 800; i++)
  // {
  //   digitalWrite(18, HIGH);
  //   delayMicroseconds(800);
  //   digitalWrite(18, LOW);
  //   delayMicroseconds(800);
  // }
  // digitalWrite(25, LOW);

  // for (int i = 0; i < 800; i++)
  // {
  //   digitalWrite(18, HIGH);
  //   delayMicroseconds(500);
  //   digitalWrite(18, LOW);
  //   delayMicroseconds(500);
  // }
  // digitalWrite(17, LOW);

  // put your main code here, to run repeatedly:
  // String* message = checkForMessage();
  // if (message != nullptr) {
  //     // Use the message...
  //     delete message;  // Don't forget to delete it!
  // }
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}