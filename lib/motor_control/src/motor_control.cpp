#include "motor_control.h"

MotorDefinition::MotorDefinition(int group, int position, int dir_pin,
                    int step_pin, int enable_pin, int uid) {
        motor_group = group;
        motor_position = position;
        motor_dir_pin = dir_pin;
        motor_step_pin = step_pin;
        motor_enable_pin = enable_pin;
        motor_uid = uid;
    };
// TO-DO Finalize the Function as done with front left
void initPlatformSteppers(MotorDefinition motor_front_left, MotorDefinition motor_front_right, MotorDefinition motor_back_left, MotorDefinition motor__back_right){
    AccelStepper stepper_front_left(AccelStepper::DRIVER, motor_front_left.getMotorStepPin(), motor_front_left.getMotorDirectionPin());
    // stepper_front_left.setEnablePin(motor_front_left.getMotorEnablePin());
    AccelStepper stepper_front_left(AccelStepper::DRIVER, motor_front_left.getMotorStepPin(), motor_front_left.getMotorDirectionPin());
    // stepper_front_left.setEnablePin(motor_front_left.getMotorEnablePin());
    AccelStepper stepper_front_left(AccelStepper::DRIVER, motor_front_left.getMotorStepPin(), motor_front_left.getMotorDirectionPin());
    // stepper_front_left.setEnablePin(motor_front_left.getMotorEnablePin());
    AccelStepper stepper_front_left(AccelStepper::DRIVER, motor_front_left.getMotorStepPin(), motor_front_left.getMotorDirectionPin());
    // stepper_front_left.setEnablePin(motor_front_left.getMotorEnablePin())
    MultiStepper platform_steppers;
    platform_steppers.addStepper(stepper_front_left);
    platform_steppers.addStepper(stepper_front_left);
    platform_steppers.addStepper(stepper_front_left);
    platform_steppers.addStepper(stepper_front_left);
};

// TO-DO Finalize the Function as done with front left
void initCentringSteppers(MotorDefinition motor_front_left, MotorDefinition motor_front_right, MotorDefinition motor_back_left, MotorDefinition motor__back_right){
    AccelStepper stepper_front_left(AccelStepper::DRIVER, motor_front_left.getMotorStepPin(), motor_front_left.getMotorDirectionPin());
    // stepper_front_left.setEnablePin(motor_front_left.getMotorEnablePin());
    AccelStepper stepper_front_left(AccelStepper::DRIVER, motor_front_left.getMotorStepPin(), motor_front_left.getMotorDirectionPin());
    // stepper_front_left.setEnablePin(motor_front_left.getMotorEnablePin());
    AccelStepper stepper_front_left(AccelStepper::DRIVER, motor_front_left.getMotorStepPin(), motor_front_left.getMotorDirectionPin());
    // stepper_front_left.setEnablePin(motor_front_left.getMotorEnablePin());
    AccelStepper stepper_front_left(AccelStepper::DRIVER, motor_front_left.getMotorStepPin(), motor_front_left.getMotorDirectionPin());
    // stepper_front_left.setEnablePin(motor_front_left.getMotorEnablePin())
    MultiStepper platform_steppers;
    platform_steppers.addStepper(stepper_front_left);
    platform_steppers.addStepper(stepper_front_left);
    platform_steppers.addStepper(stepper_front_left);
    platform_steppers.addStepper(stepper_front_left);
};

// void initRoofSteppers(){};