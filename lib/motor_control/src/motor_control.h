/**
 * @file motor_control.h
 * @brief This file contains the definition of the StepperMotor class and related constants.
 *
 * The StepperMotor class is used to control a stepper motor. It includes
 * methods for setting the motor's speed, direction, and position, as well as
 * other configuration parameters. The constants defined in this file are used
 * to configure the stepper motors.
 *
 * @author FelixKrumme
 * @date 02-06-2024
 */

#ifndef MOTOR_CONTROL
#define MOTOR_CONTROL

#include <Arduino.h>

#define MAX_MOTORS 5
#define DEFAULT_SPEED 1000
//
// All Positions should be viewed in travelling direction!!!
//

extern const int kSupportedMicroStepConfig[15];
extern const int kStepsPerRotationMicrostepping[15];
// kMaxSpeed is not known yet, (is probably also different for each motor) lower value means higher speed
const unsigned int kMaxSpeed = 500;

class StepperMotor
{
private:
    byte motor_group_;
    byte motor_position_;
    byte motor_dir_pin_;
    byte motor_step_pin_;
    byte motor_enable_pin_;
    byte motor_uid_;
    unsigned int micro_step_config_;
    unsigned int steps_per_rotation_;
    unsigned int step_count_ = 0;
    // unsigned int remaining_steps_ = 0;
    unsigned int pulse_count_ = 0;
    unsigned long current_time = 0;
    unsigned long passed_time_ = 0;
    bool direction_ = false;
    bool togglePulse_ = LOW;

public:
    StepperMotor(byte _motor_uid, byte _motor_group, byte _motor_position,
                 byte _motor_dir_pin, byte _motor_step_pin,
                 byte _motor_enable_pin,
                 unsigned int _micro_step_config);

    unsigned int getMotorGroup(void) { return motor_group_; };
    int getMotorPosition(void) { return motor_position_; };
    int getMotorDirectionPin(void) { return motor_dir_pin_; };
    int getMotorStepPin(void) { return motor_step_pin_; };
    int getMotorEnablePin(void) { return motor_enable_pin_; };
    int getMotorUniqueID(void) { return motor_uid_; };
    int getMicroStepConfig(void) { return micro_step_config_; };
    int getStepsPerRotation(void) { return steps_per_rotation_; };
    int getStepCount(void) { return step_count_; };
    // int getRemainingSteps(void) { return remaining_steps_; };

    void setMicroStepConfig(int _micro_step_config)
    {
        // Iterate over the array of supported microstep configurations.
        for (size_t i = 0; i < sizeof(kSupportedMicroStepConfig) / sizeof(kSupportedMicroStepConfig[0]); ++i)
        {
            // If _micro_step_config is in the array, gather the steps per rotation for the given microstep configuration.
            if (kSupportedMicroStepConfig[i] == _micro_step_config)
            {
                micro_step_config_ = _micro_step_config;
                steps_per_rotation_ = kStepsPerRotationMicrostepping[i];
                break;
            }
            else
            {
                if (Serial)
                {
                    // Replace with a message through the Communication Interface in serial_communication
                    Serial.println("Microstep configuration not supported.");
                }
                // Revert to basis case (halfsteps).
                micro_step_config_ = 2;
                steps_per_rotation_ = 400;
            };
        };
    };

    // void move(int speed);
    void moveByRotationsBlocking(unsigned int rotations);
    void moveByRotationsBlocking(unsigned int rotations, unsigned int speed);
    void moveByStepsBlocking(unsigned int steps);
    void moveByStepsBlocking(unsigned int steps, unsigned int speed);
};

class StepperGroup
{
private:
    byte group_id_;
    StepperMotor *motors[MAX_MOTORS];
    byte motor_count_ = 0;
    unsigned int group_speed_;
    unsigned int pulse_count_ = 0;
    unsigned int step_count_ = 0;
    unsigned int remaining_steps_ = 0;
    unsigned long passed_time_ = 0;
    bool toggle_pulse_ = LOW;
    bool direction_ = LOW;

public:
    StepperGroup(unsigned int _group_id, unsigned int _speed, bool _direction);

    void setGroupSpeed(unsigned int _speed);
    void setGroupDirection(bool _direction);
    void addMotor(StepperMotor *motor);

    void moveGroupByRotations(unsigned int rotations, bool direction);
    void moveGroupByRotations(unsigned int rotations, bool direction, unsigned int speed);
    void moveGroupBySteps(unsigned int steps, bool direction);
    void moveGroupBySteps(unsigned int steps, bool direction, unsigned int speed);

    // void moveGroup(); could be used as "non-stopping" movement as a call with a high number
};

// Use this Enumerators for assinging a Group to a Motor
enum motor_group_id
{
    motor_group_leveling,
    motor_group_big_centring,
    motor_group_small_centring,
    motor_group_roof
};

// Use this Enumerators for specifying the Position of a Motor
enum motor_position
{
    front_left,
    front_right,
    mid_left,
    mid_right,
    back_left,
    back_right,
    platform_front_left,
    platform_front_right,
    platform_mid_left,
    platform_mid_right,
    platform_back_left,
    platform_back_right,
    front_up_left,
    front_up_right

};

enum motor_uid{
    small_centring = 0,
    big_centring_front = 1,
    big_centring_back = 2,
    leveling_front_left = 3,
    leveling_front_right = 4,
    leveling_back_left = 5,
    leveling_back_right = 6
};

enum motor_direction
{
    clockwise = LOW,
    counterclockwise = HIGH
};

#endif
