#ifndef MOTOR_CONTROL
#define MOTOR_CONTROL

#include <AccelStepper.h>
#include <MultiStepper.h>

//
// All Positions should be viewed in travelling direction!!!
//

class MotorDefinition
{
private:
    int motor_group;
    int motor_position;
    int motor_dir_pin;
    int motor_step_pin;
    int motor_enable_pin;
    int motor_uid;

public:
    int getMotorGroup(void){return motor_group;};
    int getMotorPosition(void){return motor_position;};
    int getMotorDirectionPin(void){return motor_dir_pin;};
    int getMotorStepPin(void){return motor_step_pin;};
    int getMotorEnablePin(void){return motor_enable_pin;};
    int getMotorUniqueID(void){return motor_uid;};

MotorDefinition(int group, int position, int dir_pin,
                    int step_pin, int enable_pin, int uid);
    
};

// Use this Enumerators for assinging a Group to a Motor
enum motor_group_id
{
    motor_group_plattform,
    motor_group_centring,
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

// Initialize all steppers. Meaning giving them a Motor Definition and creating an Instance of AccelStepper and making them a member of a MultiStepper Instance.
// initSteppers only calls the other groups
// void initSteppers() {
//     initCentringSteppers();
//     initPlattformSteppers();
//     // initRoofSteppers();
// };

void initPlatformSteppers(MotorDefinition motor_front_left, MotorDefinition motor_front_right, MotorDefinition motor_back_left, MotorDefinition motor__back_right);
void initCentringSteppers(MotorDefinition motor_left, MotorDefinition motor_right);
void initCentringTwo();
// void initRoofSteppers(MotorDefinition motor_left, MotorDefinition motor_right);

AccelStepper stepper_front_left(AccelStepper::DRIVER, 12, 13);
AccelStepper stepper_front_right(AccelStepper::DRIVER, 14, 15);

MultiStepper platform;
void initSteppers()
{
    stepper_front_left.setMaxSpeed(100);
    stepper_front_right.setMaxSpeed(100);

    platform.addStepper(stepper_front_left);
    platform.addStepper(stepper_front_right);
}

#endif