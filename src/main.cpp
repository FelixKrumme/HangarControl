#include <Arduino.h>
// #include "serial_communication.h"
#include "motor_control.h"


// Put here Interupt Service Routines for Endtriggers

// put function declarations here:
int myFunction(int, int);
StepperMotor motor_big_centring_left = StepperMotor(0, motor_group_big_centring, platform_front_left, 12, 9, 8, 2);
StepperMotor motor_big_centring_right = StepperMotor(1, motor_group_big_centring, platform_front_right, 12, 9, 8, 2);

StepperGroup group_big_centring = StepperGroup(1, 1000, HIGH);



void setup()
{
  stepper_motors[0] = &motor_big_centring_left;
  stepper_motors[1] = &motor_big_centring_right;
  stepper_group[0] = &group_big_centring;
  Serial.begin(9600);
  while (!Serial)
  {
  };
  Serial.println("Serial Communication started..");
  pinMode(12, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  group_big_centring.addMotor(&motor_big_centring_left);
  group_big_centring.addMotor(&motor_big_centring_right);


  // put your setup code here, to run once:
}

void loop()
{
    if (Serial.available() > 0)
  {
    // read the incoming byte:
    String incomingString = Serial.readStringUntil('\n');

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingString);
    if (incomingString == "G")
    {
      group_big_centring.moveGroupByRotations(10, HIGH);
      group_big_centring.moveGroupByRotations(10, LOW);
    }
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