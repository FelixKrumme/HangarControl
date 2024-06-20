#include <Arduino.h>
// #include "serial_communication.h"
#include "motor_control.h"




// Put here Interupt Service Routines for Endtriggers


StepperMotor motor_big_centring_front = StepperMotor(big_centring_front,motor_group_big_centring, platform_front_right, 53, 52, 8, 2);
StepperMotor motor_big_centring_back = StepperMotor(big_centring_back,motor_group_big_centring, platform_back_right, 51, 50, 8, 2);

StepperGroup group_big_centring = StepperGroup(motor_group_big_centring, 4000, HIGH);


StepperMotor motor_small_centring = StepperMotor(small_centring,motor_group_small_centring, platform_mid_right, 22, 23, 8, 2);

StepperGroup group_small_centring = StepperGroup(motor_group_small_centring, 4000, HIGH);


StepperMotor motor_leveling_front_left = StepperMotor(leveling_front_left,motor_group_leveling, front_left, 47, 46, 8, 2);
StepperMotor motor_leveling_front_right = StepperMotor(leveling_front_right,motor_group_leveling, front_right, 45, 44, 8, 2);
StepperMotor motor_leveling_back_left = StepperMotor(leveling_back_left,motor_group_leveling, back_left, 43, 42, 8, 2);
StepperMotor motor_leveling_back_right = StepperMotor(leveling_back_right,motor_group_leveling, back_right, 41, 40, 8, 2);

StepperGroup group_leveling = StepperGroup(motor_group_leveling, 4000, HIGH);


void setup()
{
  Serial.begin(9600);
  for (int i = 22; i < 24; i++)
  {
    pinMode(i, OUTPUT);
  }
  for (int i = 30; i < 53; i++)
  {
    pinMode(i, INPUT);
  }

  // group_big_centring.addMotor(&motor_big_centring_front);
  // group_big_centring.addMotor(&motor_big_centring_back);

  group_small_centring.addMotor(&motor_small_centring);

  // group_leveling.addMotor(&motor_leveling_front_left);
  // group_leveling.addMotor(&motor_leveling_front_right);
  // group_leveling.addMotor(&motor_leveling_back_left);
  // group_leveling.addMotor(&motor_leveling_back_right);


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
  // Serial.println("36: ");
  // Serial.println(digitalRead(36));
  // State Machine
  // States:
  // receiving commands over serial
  // moving by switches
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
      Serial.println("Moving centring forward");
    }

    else if (incomingByte == 'b')
    {
      state = 'b';
      Serial.println("Moving centring back");
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
    // if (digitalRead(38) == HIGH)
    // {
    //   Serial.println("Moving small centring forward");
    //   group_small_centring.moveGroupBySteps(10, HIGH, 5000);
    // }
    // if (digitalRead(36) == HIGH)
    // {
    //   Serial.println("Moving small centring back");
    //   group_small_centring.moveGroupBySteps(10, LOW, 5000);
    // }
    delay(1000);
  }
  else if (state == 'f')
  {
    group_small_centring.moveGroupBySteps(11034, HIGH, 3000);
    state = 'b';
    // 43.5mm -> 5000 Steps
    // 11034 Steps -> 960mm
    Serial.println("Finished");
  }
  else if (state == 'b')
  {
    group_small_centring.moveGroupBySteps(11034, LOW, 3000);
    state = 'f';
    Serial.println("Finished");
  }
  // else if (state == 'r')
  // {
  //   group_big_centring.moveGroupBySteps(10, HIGH, 5000);
  //   group_big_centring.moveGroupBySteps(10, LOW, 5000);
  // }
  // if (digitalRead(23) == HIGH)
  // {
  //   group_big_centring.moveGroupBySteps(10, HIGH, 5000);
  // }

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