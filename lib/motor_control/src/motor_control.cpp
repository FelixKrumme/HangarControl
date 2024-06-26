#include "motor_control.h"

const int kSupportedMicroStepConfig[15] = {2, 4, 8, 16, 32, 64, 128, 5, 10, 20, 25, 40, 50, 100, 125};
const int kStepsPerRotationMicrostepping[15] = {400, 800, 1600, 3200, 6400, 12800, 25600, 1000, 2000, 4000, 5000, 8000, 10000, 20000, 25000};

/// @brief Creates a instance of the StepperMotor class used for the control and the organization
/// @param _motor_group use enum motor_group_id
/// @param _motor_position use enum motor_position
/// @param _motor_dir_pin use the Pin connceted to the DIR pin of the stepper motor driver
/// @param _motor_step_pin use the Pin connceted to the STEP/PUL pin of the stepper motor driver
/// @param _motor_enable_pin use the Pin connceted to the ENABLE pin of the stepper motor driver
/// @param _micro_step_config use the dividend for the config -> 2 for half steps etc. see the enum or datasheet for allowed values
StepperMotor::StepperMotor(byte _motor_group, byte _motor_position,
                           byte _motor_dir_pin,
                           byte _motor_step_pin,
                           byte _motor_enable_pin, unsigned int _micro_step_config)
{
    motor_group_ = _motor_group;
    motor_position_ = _motor_position;
    motor_dir_pin_ = _motor_dir_pin;
    motor_step_pin_ = _motor_step_pin;
    motor_enable_pin_ = _motor_enable_pin;
    motor_uid_ = _motor_position;
    setMicroStepConfig(_micro_step_config);
};

// With further logic this could be extended to controll multiple motors at once with different speeds etc. however the benefit isnt given at the moment
// int StepperMotor::move(int speed)
// {
//     if (remaining_steps_ > 0)
//     {
//         long current_time = millis();
//         if (current_time - passed_time_ > (long)speed)
//         {
//             // A Pulse is completed but only 2 Pulses (High + Low) make a step!
//             pulse_count_++;
//             if (pulse_count_ % 2 == 0)
//             {
//                 step_count_++;
//                 remaining_steps_--;
//             };

//             togglePulse_ = togglePulse_ == LOW ? HIGH : LOW;
//             digitalWrite(motor_step_pin_, togglePulse_);
//             passed_time_ = current_time;
//         };
//     };
// };

void StepperMotor::moveByStepsBlocking(unsigned int steps)
{
    moveByStepsBlocking(steps, DEFAULT_SPEED);
    return;
};

void StepperMotor::moveByStepsBlocking(unsigned int steps, unsigned int speed)
{
    for (unsigned int i = 0; i < steps; i++)
    {
        digitalWrite(motor_step_pin_, HIGH);
        delayMicroseconds(speed);
        digitalWrite(motor_step_pin_, LOW);
        delayMicroseconds(speed);
    }
    return;
};

void StepperMotor::moveByRotationsBlocking(unsigned int rotations)
{
    moveByRotationsBlocking(rotations, DEFAULT_SPEED);
    return;
};

void StepperMotor::moveByRotationsBlocking(unsigned int rotations, unsigned int speed)
{
    int steps = rotations * steps_per_rotation_;
    moveByStepsBlocking(steps, speed);
    return;
};

/// @brief Creates an empty instance of the StepperGroup class, which should be used for parallel movement of multiple motors.
/// @param _group_id use the enum motor_group_id
/// @param _speed sets the speed which should be the default for movements the groups make. Respect the limits.
StepperGroup::StepperGroup(unsigned int _group_id, unsigned int _speed, bool _direction)
{
    group_id_ = _group_id;
    group_speed_ = _speed; // Change to setter function
    direction_ = _direction;
};

void StepperGroup::setGroupSpeed(unsigned int _speed)
{
    if (_speed < kMaxSpeed)
    {
        group_speed_ = kMaxSpeed;
    }
    else
    {
        if (Serial)
        {
            // Replace with a message through the Communication Interface in serial_communication
            Serial.println("Speed too high, choose a lower value. Defaulting to DEFAULT Speed.");
        }
        group_speed_ = DEFAULT_SPEED;
    }
};

void StepperGroup::addMotor(StepperMotor *motor)
{
    if (motor_count_ < MAX_MOTORS)
    {
        // if (motor->getMotorGroup() != group_id_)
        // {
        //     // Replace with a message through the Communication Interface in serial_communication
        //     Serial.println("Motor should not belong to this group or is not set up correctly.");
        //     return;
        // };
        motors[motor_count_] = motor;
        motor_count_++;
        return;
    }
    else
    {
        // Replace with a message through the Communication Interface in serial_communication
        Serial.println("Maximum number of motors reached.");
    };
    return;
};

void StepperGroup::moveGroupByRotations(unsigned int rotations, bool direction)
{
    moveGroupByRotations(rotations, direction, group_speed_);
    return;
};

void StepperGroup::moveGroupByRotations(unsigned int rotations, bool direction, unsigned int speed)
{
    int steps = rotations * motors[0]->getStepsPerRotation();
    moveGroupBySteps(steps, direction, speed);
    return;
};

void StepperGroup::moveGroupBySteps(unsigned int steps, bool direction)
{
    moveGroupBySteps(steps, direction, group_speed_);
    return;
};

// Remove the duplicate function definition
// void StepperGroup::moveGroupBySteps(unsigned int steps, bool direction)
// {
//     moveGroupBySteps(steps, direction, group_speed_);
//     return;
// };

void StepperGroup::moveGroupBySteps(unsigned int steps, bool direction, unsigned int speed)
{
    // while steps are remaining wrap in while loop and use millis logic (compared to delay still will recognize interrupts! Important for endstops)
    // oder logik mit interrupt (interrupt wenn keine schritte mehr oder endstop erreicht)
    remaining_steps_ = steps;
    passed_time_ = 0;
    pulse_count_ = 0;
    step_count_ = 0;
    toggle_pulse_ = HIGH;
    Serial.print("remaining_steps_: ");
    Serial.println(remaining_steps_);
    Serial.print("steps: ");
    Serial.println(steps);
    group_speed_ = speed;
    // setGroupSpeed(speed); change line above to this setter (after its fixed)
    direction_ = direction;

    for (auto i = 0; i < motor_count_; i++)
            {
                digitalWrite(motors[i]->getMotorDirectionPin(), direction_);
            }

    while (remaining_steps_ > 0)
    {
        unsigned long current_time = micros();
        if (current_time - passed_time_ > (long)group_speed_)
        {
            pulse_count_++;
            // A Pulse is completed but only 2 Pulses (High + Low) make a step!
            if (pulse_count_ % 2 == 0)
            {
                step_count_++;
                remaining_steps_--;
            };

            toggle_pulse_ = toggle_pulse_ == LOW ? HIGH : LOW;
            for (auto i = 0; i < motor_count_; i++)
            {
                digitalWrite(motors[i]->getMotorStepPin(), toggle_pulse_);
            }

            passed_time_ = current_time;
        };
    };
    return;
};