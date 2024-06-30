#include "motor_control.h"
#include "PinChangeInterrupt.h"
#include "switch_handling.h"
#include "pin_header.h"

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

StepperGroup::StepperGroup(unsigned int _group_id, unsigned int _speed, bool _direction, int end_position)
{
    group_id_ = _group_id;
    setGroupSpeed(_speed);
    setGroupDirection(_direction);
    end_position_ = end_position;
};

void StepperGroup::setPosition(int position)
{
    position_ = position;
    return;
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

// Register Change Interrupts for the Switch too if it was a switch that triggered the movement
// Afterwards check if the Endstops are Low (Active) if so use these to consider the position
// If these are not High (not active) try to calculate the position with the steps made
void StepperGroup::moveGroupBySteps(unsigned int steps, bool direction, unsigned int speed)
{
    // while steps are remaining wrap in while loop and use millis logic (compared to delay still will recognize interrupts! Important for endstops)
    // oder logik mit interrupt (interrupt wenn keine schritte mehr oder endstop erreicht)
    remaining_steps_ = steps;
    setGroupSpeed(speed);
    bool last_direction = direction_;
    setGroupDirection(direction);
    bool inverted_direction = direction == LOW ? HIGH : LOW;
    // PNP Openers -> High Voltage if no metal is detected
    // High Voltage -> No Metal -> Interrupt Flag can be set to false
    // Metal detected -> Low Voltage => Look for Falling Edge
    // Low Voltage -> Metal detected -> Check if the Movement is in the opposite direction -> If so set the interrupt flag to false -> No need to register the endstop we're moving away from
    // Mech endstops are used with the NC configuration
    // High Voltage -> Not activated
    if (group_id_ == motor_group_small_centring)
    {
        // One Inductive Endstop/ One Mech to be added
        if (digitalRead(small_centring_endst_ind) == HIGH)
        {
            interrupt_flag_ = false;
            attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(small_centring_endst_ind), StepperGroup::isrStepperGroup, FALLING);
        }
        else if (last_direction != direction)
        {
            // Movement in opposite direction so movement should be safe
            interrupt_flag_ = false;
        }
        // if(digitalRead(small_centring_endst_mec) == HIGH){
        //     attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(small_centring_endst_mec), StepperGroup::isrStepperGroup, FALLING);
        // } else if (last_direction != direction)
        // {
        //     // Movement in opposite direction so movement should be safe
        //     interrupt_flag_ = false;
        // }
    }

    if (group_id_ == motor_group_big_centring)
    {
        // Two Inductive Endstops/ Two Mech to be added
        if (digitalRead(big_centring_front_endst_ind) == HIGH || digitalRead(big_centring_back_endst_ind) == HIGH)
        {
            interrupt_flag_ = false; // Should be remove when mech endstops are added because then they should be checked prior to setting the interrupt flag
            attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(big_centring_front_endst_ind), StepperGroup::isrStepperGroup, FALLING);
            attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(big_centring_back_endst_ind), StepperGroup::isrStepperGroup, FALLING);
        }
        else if (last_direction != direction)
        {
            // Movement in opposite direction so movement should be safe
            interrupt_flag_ = false; // Should be kept when mech endstops are added because the mech will be safe when the inductive ones are triggered
        }
        // if(digitalRead(big_centring_front_endst_mec) == HIGH || digitalRead(big_centring_back_endst_mec) == HIGH){
        //     attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(big_centring_front_endst_mec), StepperGroup::isrStepperGroup, FALLING);
        //     attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(big_centring_back_endst_mec), StepperGroup::isrStepperGroup, FALLING);
        // } else if (last_direction != direction)
        // {
        //     // Movement in opposite direction so movement should be safe
        //     interrupt_flag_ = false;
        // }
    }

    if (group_id_ == motor_group_leveling)
    {
        // Four Inductive Endstops/ Four Mech to be added
        if (digitalRead(leveling_front_left_endst_ind) == HIGH || digitalRead(leveling_front_right_endst_ind) == HIGH || digitalRead(leveling_back_left_endst_ind) == HIGH || digitalRead(leveling_back_right_endst_ind) == HIGH)
        {
            interrupt_flag_ = false;
            attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(leveling_front_left_endst_ind), StepperGroup::isrStepperGroup, FALLING);
            attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(leveling_front_right_endst_ind), StepperGroup::isrStepperGroup, FALLING);
            attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(leveling_back_left_endst_ind), StepperGroup::isrStepperGroup, FALLING);
            attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(leveling_back_right_endst_ind), StepperGroup::isrStepperGroup, FALLING);
        }
        else if (last_direction != direction)
        {
            // Movement in opposite direction so movement should be safe
            interrupt_flag_ = false;
        }
        // if(digitalRead(leveling_front_left_endst_mec) == HIGH || digitalRead(leveling_front_right_endst_mec) == HIGH || digitalRead(leveling_back_left_endst_mec) == HIGH || digitalRead(leveling_back_right_endst_mec) == HIGH){
        //     attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(leveling_front_left_endst_mec), StepperGroup::isrStepperGroup, FALLING);
        //     attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(leveling_front_right_endst_mec), StepperGroup::isrStepperGroup, FALLING);
        //     attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(leveling_back_left_endst_mec), StepperGroup::isrStepperGroup, FALLING);
        //     attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(leveling_back_right_endst_mec), StepperGroup::isrStepperGroup, FALLING);
        // } else if (last_direction != direction)
        // {
        //     // Movement in opposite direction so movement should be safe
        //     interrupt_flag_ = false;
        // }
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
                if (motors[i]->getMotorUniqueID() % 2 != 0)
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
    // detachPinChangeInterrupt(digitalPinToPinChangeInterrupt(40));
    return;
};

void StepperGroup::switchMoveGroupBySteps(unsigned int steps, bool direction, unsigned int speed)
{
    remaining_steps_ = steps;
    setGroupSpeed(speed);
    setGroupDirection(direction);

    
     // while steps are remaining wrap in while loop and use millis logic (compared to delay still will recognize interrupts! Important for endstops)
    // oder logik mit interrupt (interrupt wenn keine schritte mehr oder endstop erreicht)
    remaining_steps_ = steps;
    setGroupSpeed(speed);
    bool last_direction = direction_;
    setGroupDirection(direction);
    bool inverted_direction = direction == LOW ? HIGH : LOW;
    // No endstop logic need when actions are manually 

    while (remaining_steps_ > 0)
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
                if (motors[i]->getMotorUniqueID() % 2 != 0)
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
    return;
}

// Higher Level Moving Functions

void moveBigCentringLeft(StepperGroup group_big_centring)
{
    int moving_distance = group_big_centring.getEndPosition() - group_big_centring.getPosition();
    unsigned int steps = round(moving_distance * 11.76);
    group_big_centring.moveGroupBySteps(steps, HIGH, 3000);
    group_big_centring.setPosition(group_big_centring.getEndPosition());
    return;
};

void moveBigCentringRight(StepperGroup group_big_centring)
{
    //   int moving_distance = abs(group_big_centring.getPosition() - group_big_centring.getEndPosition());
    //     unsigned int steps = round(moving_distance * 11.76);
    group_big_centring.moveGroupBySteps(NULL, LOW, 3000);
    group_big_centring.setPosition(0);
    return;
};

void homeBigCentring(StepperGroup group_big_centring)
{
    // Move until (inductive) endstop is reached
    // Set position to 0 (afterward use hard coded step ammount to move to the other side)
    // (Next Step) move to the other endstops position
    // Save steps needed to reach the endstop
    // Move to ind endstops
    // Save steps needed to reach the endstop
    group_big_centring.moveGroupBySteps(NULL, LOW, 3000);
    group_big_centring.setPosition(0);
    return;
};

// Function to move the small centring back and forth
// measured distances (results in 11.49 steps per mm)
// 435mm -> 5000 Steps
// 11034 Steps -> 960mm

void moveSmallCentringBack(StepperGroup group_small_centring)
{
    int moving_distance = group_small_centring.getEndPosition() - group_small_centring.getPosition();
    unsigned int steps = round(moving_distance * 11.49);
    group_small_centring.moveGroupBySteps(steps, HIGH, 3000);
    group_small_centring.setPosition(group_small_centring.getEndPosition());
    return;
};

void moveSmallCentringForward(StepperGroup group_small_centring)
{
    // int moving_distance = abs(group_small_centring.getPosition() - group_small_centring.getEndPosition());
    // unsigned int steps = round(moving_distance * 11.49);
    group_small_centring.moveGroupBySteps(11040, LOW, 3000);
    group_small_centring.setPosition(0);
    return;
};

void homeSmallCentring(StepperGroup group_small_centring)
{
    // Simplified homing which only sets the position to 0 when it runs into the endstop. Could be extended by using the other endstop to calculate the distance
    group_small_centring.moveGroupBySteps(11100, HIGH, 3000);
    group_small_centring.setPosition(0);
    return;
};

// Function to move the platform up and down

void movePlatformUp(StepperGroup group_leveling)
{
    int moving_distance = group_leveling.getEndPosition() - group_leveling.getPosition();
    unsigned int steps = round(moving_distance * NULL);
    group_leveling.moveGroupBySteps(steps, HIGH, 3000);
    group_leveling.setPosition(group_leveling.getEndPosition());
    return;
};

void movePlatformDown(StepperGroup group_leveling)
{
    // int moving_distance = abs(group_leveling.getPosition() - group_leveling.getEndPosition());
    // unsigned int steps = round(moving_distance * NULL);
    group_leveling.moveGroupBySteps(NULL, LOW, 3000);
    group_leveling.setPosition(0);
    return;
};

void homePlatform(StepperGroup group_leveling)
{
    // Simplified homing which only sets the position to 0 when it runs into the endstop. Could be extended by using the other endstop to calculate the distance
    group_leveling.moveGroupBySteps(NULL, LOW, 3000);
    group_leveling.setPosition(0);
    return;
};

// Idea extended homing:
// Move until (inductive) endstop is reached
// Set position to 0 (afterward use hard coded step ammount to move to the other side)
// (Next Step) move to the other endstops position
// Save steps needed to reach the endstop
// Move to ind endstops
// Save steps needed to reach the endstop