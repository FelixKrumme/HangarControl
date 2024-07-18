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

void setupSmallCentring();
void setupBigCentring();
void setupLeveling();

unsigned int small_centring_steps = 0;
unsigned int big_centring_steps = 0;
unsigned int leveling_steps = 0;

void setup()
{
  Serial.begin(9600);
  // Configure the pins accordingly to the pin_header.h
  for (int i = 22; i < 36; i++)
  {
    pinMode(i, OUTPUT);
  }
  for (int i = 50; i < 64; i++)
  {
    pinMode(i, INPUT);
  }
  for (int i = 12; i < 22; i++)
  {
    pinMode(i, INPUT);
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
  // if (digitalRead(big_centring_back_endst_ind) == HIGH)
  // {
  //   Serial.println("Endstop Big Centring Back triggered");
  // }
  // if (digitalRead(big_centring_front_endst_ind) == HIGH)
  // {
  //   Serial.println("Endstop Big Centring Front triggered");
  // }
}

char state = 'p';
unsigned int steps_needed = 0;

void loop()
{
  if (Serial.available() > 0)
  {
    String incoming = Serial.readStringUntil('\n');
    Serial.println(incoming);
    processIncomingCommand(incoming);
  }

  while (digitalRead(switch_leveling_left) == HIGH)
  {
    group_leveling.switchMoveGroupBySteps(10, HIGH, PLATFORM_DOWN_SPEED);
  }

  while (digitalRead(switch_leveling_right) == HIGH)
  {
    group_leveling.switchMoveGroupBySteps(10, LOW, PLATFORM_UP_SPEED);
  }

  while (digitalRead(switch_big_centring_left) == HIGH)
  {
    group_big_centring.switchMoveGroupBySteps(10, LOW, BIG_CENTRING_LEFT_SPEED);
  }

  while (digitalRead(switch_big_centring_right) == HIGH)
  {
    group_big_centring.switchMoveGroupBySteps(10, HIGH, BIG_CENTRING_RIGHT_SPEED);
  }

  while (digitalRead(switch_small_centring_left) == HIGH)
  {
    group_small_centring.switchMoveGroupBySteps(10, LOW, SMALL_CENTRING_BACKWARD_SPEED);
  }

  while (digitalRead(switch_small_centring_right) == HIGH)
  {
    group_small_centring.switchMoveGroupBySteps(10, HIGH, SMALL_CENTRING_FORWARD_SPEED);
  }
}

void processIncomingCommand(const String &incoming)
{
  if (incoming.length() < 3)
    return; // Ensure the string is long enough

  if (incoming.startsWith("M"))
  {
    char motorGroup = incoming[1];
    bool direction = incoming[2] == '1';

    switch (motorGroup)
    {
    case 'L': // Leveling
      if (direction)
      {
        group_leveling.moveGroupBySteps(leveling_steps, LOW, PLATFORM_UP_SPEED);
        // movePlatformUp(group_leveling);
      }
      else
      {
        group_leveling.moveGroupBySteps(leveling_steps, HIGH, PLATFORM_DOWN_SPEED);
        // movePlatformDown(group_leveling);
      }
      break;
    case 'B': // Big Centring
      if (direction)
      {
        group_big_centring.moveGroupBySteps(big_centring_steps, LOW, BIG_CENTRING_RIGHT_SPEED);
      }
      else
      {
        // moveBigCentringRight(group_big_centring);
        group_big_centring.moveGroupBySteps(big_centring_steps, HIGH, BIG_CENTRING_LEFT_SPEED);

      }
      break;
    case 'S': // Small Centring
      if (direction)
      {
        // moveSmallCentringForward(group_small_centring);
        group_small_centring.moveGroupBySteps(small_centring_steps, LOW, SMALL_CENTRING_BACKWARD_SPEED);

      }
      else
      {
        // moveSmallCentringBack(group_small_centring);
        group_small_centring.moveGroupBySteps(small_centring_steps, HIGH, SMALL_CENTRING_FORWARD_SPEED);
      }
      break;
    case 'A': // Automated
      if (direction)
      {
        group_small_centring.moveGroupBySteps(small_centring_steps, LOW, SMALL_CENTRING_FORWARD_SPEED);
        group_big_centring.moveGroupBySteps(big_centring_steps, LOW, BIG_CENTRING_RIGHT_SPEED);
        group_leveling.moveGroupBySteps(leveling_steps, HIGH, PLATFORM_DOWN_SPEED);
      }
      else
      {
        group_small_centring.moveGroupBySteps((small_centring_steps*1.2), HIGH, SMALL_CENTRING_FORWARD_SPEED);
        group_big_centring.moveGroupBySteps(big_centring_steps, HIGH, BIG_CENTRING_LEFT_SPEED);
        group_leveling.moveGroupBySteps(leveling_steps, LOW, PLATFORM_UP_SPEED);
      }
      break;
    case 'I': // Initialize
      if (direction){
        // moveSmallCentringForward(group_small_centring);
        setupSmallCentring();
        Serial.println("Steps calculated for small centring: ");
        Serial.println(small_centring_steps);
        setupBigCentring();
        Serial.println("Steps calculated for big centring: ");
        Serial.println(big_centring_steps);
        setupLeveling();
        Serial.println("Steps calculated for leveling: ");
        Serial.println(leveling_steps);
      } else {
        Serial.println("Initialization values: ");
        Serial.println("Small centring: ");
        Serial.println(small_centring_steps);
        Serial.println("Big centring: ");
        Serial.println(big_centring_steps);
        Serial.println("Leveling: ");
        Serial.println(leveling_steps);
      }
      break;
    // case 'R': // Roof
    //   // Assuming switchMoveRoofOpen() and switchMoveRoofClose() are uncommented and implemented
    //   if (direction)
    //   {
    //     // switchMoveRoofOpen();
    //   }
    //   else
    //   {
    //     // switchMoveRoofClose();
    //   }
    //   break;
    default:
      Serial.println("Invalid motor group");
      break;
    }
  }
}

void setupSmallCentring()
{
  unsigned int steps_made = 0;
  while (digitalRead(small_centring_endst_ind) == HIGH)
  {
    group_small_centring.moveGroupBySteps(10, LOW, SMALL_CENTRING_FORWARD_SPEED);
    steps_made += 10;
  }
  small_centring_steps = steps_made;
  return;
}

void setupBigCentring()
{
  unsigned int steps_made = 0;
  while (digitalRead(big_centring_front_endst_ind) == HIGH && digitalRead(big_centring_back_endst_ind) == HIGH)
  {
    group_big_centring.moveGroupBySteps(10, LOW, BIG_CENTRING_RIGHT_SPEED);
    steps_made += 10;
  }
  big_centring_steps = steps_made;
  return;
}

void setupLeveling(){
  unsigned int steps_made = 0;
  while (digitalRead(leveling_front_left_endst_ind) == HIGH && digitalRead(leveling_front_right_endst_ind) == HIGH && digitalRead(leveling_back_left_endst_ind) == HIGH && digitalRead(leveling_back_right_endst_ind) == HIGH)
  {
    group_leveling.moveGroupBySteps(10, HIGH, PLATFORM_DOWN_SPEED);
    steps_made += 10;
  }
  leveling_steps = steps_made;
  return;
}