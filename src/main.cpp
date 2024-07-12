#include <Arduino.h>
// #include "serial_communication.h"
#include "motor_control.h"
#include "pin_header.h"

StepperMotor motor_big_centring_front = StepperMotor(big_centring_front, motor_group_big_centring, platform_front_right, big_centring_front_dir_pin, big_centring_front_pul_pin, 99, 2);
StepperMotor motor_big_centring_back = StepperMotor(big_centring_back, motor_group_big_centring, platform_back_right, big_centring_back_dir_pin, big_centring_back_pul_pin, 99, 2);

StepperGroup group_big_centring = StepperGroup(motor_group_big_centring, 4000, HIGH, 1350);

StepperMotor motor_small_centring = StepperMotor(small_centring, motor_group_small_centring, platform_mid_right, small_centring_dir_pin, small_centring_pul_pin, 99, 2);

StepperGroup group_small_centring = StepperGroup(motor_group_small_centring, 4000, HIGH, 960);

StepperMotor motor_leveling_front_left = StepperMotor(leveling_front_left, motor_group_leveling, front_left, leveling_front_left_dir_pin, leveling_front_left_pul_pin, 99, 2);
StepperMotor motor_leveling_front_right = StepperMotor(leveling_front_right, motor_group_leveling, front_right, leveling_front_right_dir_pin, leveling_front_right_pul_pin, 99, 2);
StepperMotor motor_leveling_back_left = StepperMotor(leveling_back_left, motor_group_leveling, back_left, leveling_back_left_dir_pin, leveling_back_left_pul_pin, 99, 2);
StepperMotor motor_leveling_back_right = StepperMotor(leveling_back_right, motor_group_leveling, back_right, leveling_back_right_dir_pin, leveling_back_right_pul_pin, 99, 2);

StepperGroup group_leveling = StepperGroup(motor_group_leveling, 4000, HIGH, 0);

// unsigned int switchMoveBigCentringLeft();
// unsigned int switchMoveBigCentringRight();
// unsigned int switchMoveSmallCentringFront();
// unsigned int switchMoveSmallCentringBack();
// unsigned int switchMoveLevelingUp();
// unsigned int switchMoveLevelingDown();
// unsigned int switchMoveRoofOpen();
// unsigned int switchMoveRoofClose();
void processIncomingCommand(const String &incoming);

void setup()
{
  Serial.begin(9600);
  // Configure the pins accordingly to the pin_header.h
  for (int i = 22; i < 54; i++)
  {
    pinMode(i, OUTPUT);
  }
  // for (int i = 30; i < 53; i++)
  // {
  //   pinMode(NULL, INPUT);
  // }

  // Register the Motors to the Groups
  group_big_centring.addMotor(&motor_big_centring_front);
  group_big_centring.addMotor(&motor_big_centring_back);

  group_small_centring.addMotor(&motor_small_centring);

  group_leveling.addMotor(&motor_leveling_front_left);
  group_leveling.addMotor(&motor_leveling_front_right);
  group_leveling.addMotor(&motor_leveling_back_left);
  group_leveling.addMotor(&motor_leveling_back_right);

  // 10000 Steps 876mm
  // group_big_centring.moveGroupBySteps(10000, HIGH, 5000);
  // 400mm -> 4566(.2) Steps
  // group_big_centring.moveGroupBySteps(4566, LOW, 5000); // 293mm
  // Sweet Spot of Speed for big centring probably is somewhere between 2000 and 4000

  Serial.println("Starting program");
}

char state = 'p';

void loop()
{
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
      Serial.println("Moving big centring to the left");
    }

    else if (incomingByte == 'b')
    {
      state = 'b';
      Serial.println("Moving big centring to the right");
    }
  }
    // If it's an 'r', resume the program
    // else if (incomingByte == 'r')
    // {
    //   state = 'r';
    //   Serial.println("Moving back and forth");
    // }
    // }
    // if (Serial.available() > 0)
    // {
    //   String incoming = Serial.readStringUntil('\n');
    //   processIncomingCommand(incoming);
    // }

    // Button Movements
    while (digitalRead(switch_big_centring_left) == HIGH)
    {
      group_big_centring.switchMoveGroupBySteps(10, HIGH, 5000);
    }

    while (digitalRead(switch_big_centring_right) == HIGH)
    {
      group_big_centring.switchMoveGroupBySteps(10, LOW, 5000);
    }

    while (digitalRead(switch_small_centring_left) == HIGH)
    {
      group_small_centring.switchMoveGroupBySteps(10, HIGH, 5000);
    }

    while (digitalRead(switch_small_centring_right) == HIGH)
    {
      group_small_centring.switchMoveGroupBySteps(10, LOW, 5000);
    }

    // while (digitalRead(switch_leveling_left) == HIGH)
    if (state == 'f')
    {
      group_leveling.switchMoveGroupBySteps(10, HIGH, 650);
    }

    // while (digitalRead(switch_leveling_right) == HIGH)
    if (state == 'b')
    {
      // (Low === Hoch bei Plattform)
      group_leveling.switchMoveGroupBySteps(10, LOW, 400);
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
    // Idea for an extended Messaging Protocol (to be implemented in the future)
    // String* message = checkForMessage();
    // if (message != nullptr) {
    //     // Use the message...
    //     delete message;  // Don't forget to delete it!
    // }

  // Interrupt Var and ISR for the position tracking with switches
  // bool loop_flag = true;
  // void isrResetLoopFlag();

  // With this implementation there isnt a need for the switchMoveBigCentringLeft() function as the End could be triggered with the switch interrupt also
  // This makes it more of a design choice if switch moves should obey to endstops or not
  // Currently the switch moves are not obeying to the endstops making it easier to take fully manual control of the movements.
  // In the future when the behavior of the movements is enoughly tested and the endstops are implemented correctly the switch moves could be changed to obey to the endstops making it safer to avoid unwanted movements going over the limits.

  // Experimental for keeping the position system when moving with the switches
  // Create similar functions for the other switches if its working
  // Keep in mind that there are only 6 Interrupt Pins which can be used
  // Configure accordingly in the pin_header.h
  // void switchMoveBigCentringLeft(){
  //   loop_flag = true;
  //   unsigned int steps_made = 0;
  //   // register endstop interrupts
  //   attachInterrupt(digitalPinToInterrupt(switch_big_centring_left), isrResetLoopFlag, FALLING);
  //   while(loop_flag){
  //     group_big_centring.moveGroupBySteps(10, HIGH, 5000);
  //     steps_made += 10;
  //   }
  //   // update position
  //   group_big_centring.setPosition(group_big_centring.getPosition() + (steps_made * 0.293)); // 293mm per 10 steps check that number because its probably not exact
  //   // deregister interrupt
  //   detachInterrupt(digitalPinToInterrupt(switch_big_centring_left));
  //   return;
  // }

  // void isrResetLoopFlag(){
  //   loop_flag = false;
  // }
}
//   void processIncomingCommand(const String &incoming)
//   {
//     if (incoming.length() < 3)
//       return; // Ensure the string is long enough

//     if (incoming.startsWith("M"))
//     {
//       char motorGroup = incoming[1];
//       bool direction = incoming[2] == '1';

//       switch (motorGroup)
//       {
//       case 'L': // Leveling
//         if (direction)
//         {
//           movePlatformUp(group_leveling);
//         }
//         else
//         {
//           movePlatformDown(group_leveling);
//         }
//         break;
//       case 'B': // Big Centring
//         if (direction)
//         {
//           moveBigCentringLeft(group_big_centring);
//         }
//         else
//         {
//           moveBigCentringRight(group_big_centring);
//           ;
//         }
//         break;
//       case 'S': // Small Centring
//         if (direction)
//         {
//           moveSmallCentringForward(group_small_centring);
//         }
//         else
//         {
//           moveSmallCentringBack(group_small_centring);
//         }
//         break;
//       // case 'R': // Roof
//       //   // Assuming switchMoveRoofOpen() and switchMoveRoofClose() are uncommented and implemented
//       //   if (direction)
//       //   {
//       //     // switchMoveRoofOpen();
//       //   }
//       //   else
//       //   {
//       //     // switchMoveRoofClose();
//       //   }
//       //   break;
//       default:
//         Serial.println("Invalid motor group");
//         break;
//       }
//     }
//   }
// }