/**
 * @file serial_communication.h
 * @brief This file contains the definition of the \todo class and defines the commands that can be sent to the Arduino.
 *
 * This should basically follow a "G-Code" like style meaning commands exist of a certain Group letter, Command number and parameters. Altough heavily adapted to the specific problem and the motor_control unit. The \todo class is used to \todo. The commands defined in this file are used to \todo.
 *
 * @author \todo
 * @date \todo
 */

#ifndef SERIAL_COMMUNICATION
#define SERIAL_COMMUNICATION

#include <Arduino.h>
#include <motor_control.h>

// Command Structure
// Command: <Command Group><Command Number> <Parameters>
// Command Group: G Move, M Message
// Commands:
// G0 Blocking Move
// G1 Non-Blocking Move
// M0 Message

// G0 Blocking Move Parameters
// G0 [G] ID S/R<rate> D<direction> [V<speed>]

class CommandParser
{
public:
    bool parse(const String &command)
    {
        char commandLetter;
        int commandNumber;
        char optionalG = '\0';
        int id;
        char rateLetter;
        int rate;
        char directionLetter;
        int direction;
        char optionalV = '\0';
        int speed;

        int itemsScanned = sscanf(command.c_str(),
                                  "%c%d %c %d %c%d %c%d %c%d",
                                  &commandLetter,
                                  &commandNumber,
                                  &optionalG,
                                  &id,
                                  &rateLetter,
                                  &rate,
                                  &directionLetter,
                                  &direction,
                                  &optionalV,
                                  &speed);

        // Check if the command starts with 'G' and has at least 7 items (without optional parts)
        if (commandLetter == 'G' && itemsScanned >= 7)
        {
            // Check if the rateLetter is either 'S' or 'R' and directionLetter is 'D'
            switch (commandNumber)
            {
            case 0:
                // Blocking Move
                if (optionalG)
                {
                    if (Serial)
                    {
                        Serial.println("Invalid Command, no blocking move allowed in group mode");
                    }
                    return false;
                }
                else
                {
                    if (rateLetter == 'S')
                    {
                        optionalV ? stepper_motors[id]->moveByRotationsBlocking(rate, speed) : stepper_motors[id]->moveByRotationsBlocking(rate);
                    }
                    else if (rateLetter == 'R')
                    {
                        optionalV ? stepper_motors[id]->moveByStepsBlocking(rate, speed) : stepper_motors[id]->moveByStepsBlocking(rate);
                    }
                    else
                    {
                        return false;
                    }
                }
                break;
            case 1:
                if (optionalG)
                {
                    // Group Move
                    if (rateLetter == 'S')
                    {
                        optionalV ? stepper_group[id]->moveGroupByRotations(rate, direction, speed) : stepper_group[id]->moveGroupByRotations(rate, direction);
                    }
                    else if (rateLetter == 'R')
                    {
                        optionalV ? stepper_group[id]->moveGroupBySteps(rate, direction, speed) : stepper_group[id]->moveGroupBySteps(rate, direction);
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    // Single Move non Blocking not implemented
                    if (Serial)
                    {
                        Serial.println("Non-Blocking Move not yet implemented for single Motors.");
                    }
                    return false;
                }

                break;
                // Non-Blocking Move
                // case 28:
                //   homing
                //   break;

            default:
                if (Serial)
                {
                    Serial.print("Invalid Command.");
                }
                return false;
            }
        }
        return false;
    }
};

#endif
