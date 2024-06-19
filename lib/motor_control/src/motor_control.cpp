#include "motor_control.h"
// #include "switches_pin_header.h"
#include "PinChangeInterrupt.h"

const int kSupportedMicroStepConfig[15] = {2, 4, 8, 16, 32, 64, 128, 5, 10, 20, 25, 40, 50, 100, 125};
const int kStepsPerRotationMicrostepping[15] = {400, 800, 1600, 3200, 6400, 12800, 25600, 1000, 2000, 4000, 5000, 8000, 10000, 20000, 25000};

/// @brief Creates a instance of the StepperMotor class used for the control and the organization
/// @param _motor_group use enum motor_group_id
/// @param _motor_position use enum motor_position
/// @param _motor_dir_pin use the Pin connceted to the DIR pin of the stepper motor driver
/// @param _motor_step_pin use the Pin connceted to the STEP/PUL pin of the stepper motor driver
/// @param _motor_enable_pin use the Pin connceted to the ENABLE pin of the stepper motor driver
/// @param _micro_step_config use the dividend for the config -> 2 for half steps etc. see the enum or datasheet for allowed values
StepperMotor::StepperMotor(byte _motor_uid, byte _motor_group, byte _motor_position,
                           byte _motor_dir_pin,
                           byte _motor_step_pin,
                           byte _motor_enable_pin, unsigned int _micro_step_config)
{
    motor_uid_ = _motor_uid;
    motor_group_ = _motor_group;
    motor_position_ = _motor_position;
    motor_dir_pin_ = _motor_dir_pin;
    motor_step_pin_ = _motor_step_pin;
    motor_enable_pin_ = _motor_enable_pin;
    micro_step_config_ = _micro_step_config;
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
volatile bool StepperGroup::interrupt_flag_ = false;

StepperGroup::StepperGroup(unsigned int _group_id, unsigned int _speed, bool _direction)
{
    group_id_ = _group_id;
    setGroupSpeed(_speed);
    setGroupDirection(_direction);
    // interrupt_flag_ = false;
};

void StepperGroup::setGroupSpeed(unsigned int _speed)
{
    if (_speed > kMaxSpeed)
    {
        group_speed_ = _speed;
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

void StepperGroup::setGroupDirection(bool _direction)
{
    if (_direction == HIGH)
    {
        direction_ = HIGH;
    }
    else
    {
        direction_ = LOW;
    }
};

void StepperGroup::addMotor(StepperMotor *motor)
{
    if (motor_count_ < MAX_MOTORS)
    {
        if (motor->getMotorGroup() != group_id_)
        {
            // Replace with a message through the Communication Interface in serial_communication
            Serial.println("Motor should not belong to this group or is not set up correctly.");
            return;
        };
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
    setGroupSpeed(speed);
    bool last_direction = direction_;
    setGroupDirection(direction);
    bool inverted_direction = direction == LOW ? HIGH : LOW;
    // if (group_id_ == motor_group_leveling){
    //     // Check if the pins connected to the endstops are low if not the interrupt flag should be set to false again
    //         attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(leveling_front_left_endst_ind), StepperGroup::isrStepperGroup, FALLING);
    //         attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(leveling_front_right_endst_ind), StepperGroup::isrStepperGroup, FALLING);
    //         // Complete for all cases after testing
    //     }
    if (group_id_ == motor_group_small_centring){
        // Check if the pins connected to the endstops are low if not the interrupt flag should be set to false again
        if(digitalRead(52) == HIGH){
            interrupt_flag_ = false;
            // attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(big_centring_front_endst_ind), StepperGroup::isrStepperGroup, FALLING);
            // attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(big_centring_back_endst_ind), StepperGroup::isrStepperGroup, FALLING);
            attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(52), StepperGroup::isrStepperGroup, FALLING);
            // Complete for all cases after testing
        } else if (last_direction != direction)
        {
            // Movement in opposite direction so movement should be safe
            interrupt_flag_ = false;
        }
        
    }
    while (remaining_steps_ > 0 && !interrupt_flag_)
    {
        unsigned long current_time = micros();
        if (current_time - passed_time_ > (long)group_speed_)
        {
            // A Pulse is completed but only 2 Pulses (High + Low) make a step!
            if (pulse_count_ % 2 == 0)
            {
                step_count_++;
                remaining_steps_--;
            };

            toggle_pulse_ = toggle_pulse_ == LOW ? HIGH : LOW;
            for (auto i = 0; i < motor_count_; i++)
            {
                // Temporary solution for the (inverted directions of motors on opposite sides.
                // Serial.println("i: ");
                // Serial.println(i);
                // Serial.println("Direction: ");
                // Serial.println(direction);
                if (motors[i]-> getMotorUniqueID() % 2 != 0)
                {
                    digitalWrite(motors[i]->getMotorDirectionPin(), inverted_direction);
                    // Serial.println("Direction inverted: ");
                    // Serial.println(direction);
                }
                else
                {
                    digitalWrite(motors[i]->getMotorDirectionPin(), direction);
                }

                // digitalWrite(motors[i]->getMotorDirectionPin(), direction);
                digitalWrite(motors[i]->getMotorStepPin(), toggle_pulse_);
            }

            passed_time_ = current_time;
        };
    };
    detachPinChangeInterrupt(digitalPinToPinChangeInterrupt(40));
    return;
};