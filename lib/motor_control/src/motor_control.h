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
 * @date 18-07-2024
 */

#ifndef MOTOR_CONTROL
#define MOTOR_CONTROL

#include <Arduino.h>

#define MAX_MOTORS 5

#define DEFAULT_SPEED 1000
#define SMALL_CENTRING_FORWARD_SPEED 2000
#define SMALL_CENTRING_BACKWARD_SPEED 2000
#define BIG_CENTRING_LEFT_SPEED 2000
#define BIG_CENTRING_RIGHT_SPEED 1750
#define PLATFORM_UP_SPEED 500
#define PLATFORM_DOWN_SPEED 700
//
// All Positions should be viewed in travelling direction!!!
//

extern const int kSupportedMicroStepConfig[15];
extern const int kStepsPerRotationMicrostepping[15];
// kMaxSpeed is not known yet, (is probably also different for each motor) lower value means higher speed
const unsigned int kMaxSpeed = 100;
// Timeout threshold for the stepper motor in milliseconds.
const unsigned int kTimeoutThreshold = 5000; // tbd

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
    unsigned int step_count_;
    // unsigned int remaining_steps_ = 0;
    unsigned int pulse_count_;
    unsigned long current_time;
    unsigned long passed_time_;
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
    int position_ = 0; // Position in mm
    int end_position_ = 0;   // Position in mm
    int distance_in_steps = 0; // Distance between 0 and end_position in steps
    static volatile bool interrupt_flag_;

public:
    StepperGroup(unsigned int _group_id, unsigned int _speed, bool _direction,int end_position); 

    void setGroupSpeed(unsigned int _speed);
    void setGroupDirection(bool _direction);

    void setPosition(int position); 
    int getPosition(void) {return position_;};
    int getEndPosition(void) {return end_position_;};

    void addMotor(StepperMotor *motor);

    void moveGroupByRotations(unsigned int rotations, bool direction);
    void moveGroupByRotations(unsigned int rotations, bool direction, unsigned int speed);
    void moveGroupBySteps(unsigned int steps, bool direction);
    void moveGroupBySteps(unsigned int steps, bool direction, unsigned int speed);
    void moveGroupBySteps(unsigned int steps, bool direction, unsigned int speed, bool timeout_active);
    void switchMoveGroupBySteps(unsigned int steps, bool direction, unsigned int speed);

    void resetRemainingSteps () {
        remaining_steps_ = 0;
    }

    static void isrStepperGroup(){
        interrupt_flag_ = true;
    };
    
};

void moveBigCentringLeft(StepperGroup group_big_centring);
void moveBigCentringRight(StepperGroup group_big_centring);
void homeBigCentring(StepperGroup group_big_centring);

void moveSmallCentringBack(StepperGroup group_small_centring);
void moveSmallCentringForward(StepperGroup group_small_centring);
void homeSmallCentring(StepperGroup group_small_centring);

void movePlatformUp(StepperGroup group_leveling);
void movePlatformDown(StepperGroup group_leveling);
void homePlatform(StepperGroup group_leveling);

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

// Even Number means there wont be a inverveted direction
enum motor_uid{
    small_centring = 0,
    big_centring_front = 1,
    big_centring_back = 2,
    leveling_front_left = 4,
    leveling_front_right = 6,
    leveling_back_left = 8,
    leveling_back_right = 10
};

enum motor_direction
{
    clockwise = LOW,
    counterclockwise = HIGH
};

#endif
