#include <Arduino.h>
// #include "serial_communication.h"
#include "motor_control.h"

// Put here Interupt Service Routines for Endtriggers

// put function declarations here:
int myFunction(int, int);
StepperMotor stepper1 = StepperMotor(motor_group_plattform, front_left, 12, 9, 8, 2);
StepperGroup group1 = StepperGroup(1, 1000, HIGH);



void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
  };
  Serial.println("Serial Communication started..");
  pinMode(12, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  group1.addMotor(&stepper1);


  // put your setup code here, to run once:
}

void loop()
{
  // digitalWrite(12, HIGH);
  // stepper1.moveByStepsBlocking(400, 5000);
  // digitalWrite(12, LOW);
  // stepper1.moveByStepsBlocking(800, 4000);
  // delay(1000);

  group1.moveGroupByRotations(10, HIGH);

  group1.moveGroupByRotations(10, LOW);


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