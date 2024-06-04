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

// Command Structure
// Command: <Command Group><Command Number> <Parameters>
// Command Group: G Move, M Message
// Commands:
// G0 Blocking Move
// G1 Non-Blocking Move
// M0 Message

// G0 Blocking Move Parameters
// G0 [G] ID S/R<rate> D<direction> [V<speed>] 

#endif
