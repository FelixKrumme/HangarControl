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

// For simplicity and out of memory concerns the system will not store a queue of messages. It will only store the last message and execute it. During the execution there will be no new messages accepted. To handle this specification properly the system should return a success message or error message when a command was executed.

// Command Structure
// Command: <Command Group><Command Number> <Parameters>
// Command Group: G Move, M Message, P Parameter Setting
// Commands:
// G0 Blocking Move
// G1 Non-Blocking Move
// P Parameter Setting could be used to change Settings of eg a StepperMotor Class
// M0 Message (will probably not be used as there is currently no point to receive a message with the Microcontroller)

// G0 Blocking Move Parameters
// G0 [G] ID S/R<rate> D<direction> [V<speed>]

// G0 Blocking Move Parameters
// G0 [G] ID S/R<rate> D<direction> [V<speed>]

// G0 Blocking Move Parameters
// G0 [G] ID S/R<rate> D<direction> [V<speed>]

class SerialMessage
{
private:
    // byte message_id_; Should be used in future when multiple messages could be handled.
    String message_;

public:
    SerialMessage::SerialMessage(String _message);
    String* createMessage();
    void sendMessage(SerialMessage message_);
    // byte createMessageID();
};

class SerialErrorMessage : public SerialMessage
{
    public:
    SerialErrorMessage::SerialErrorMessage(String _message);
    void sendMessage(SerialErrorMessage message_);
};

class SerialInfoMessage : public SerialMessage
{
    public:
    SerialInfoMessage::SerialInfoMessage(String _message);
    void sendMessage(SerialInfoMessage message_);
};

class SerialResponseMessage : public SerialMessage
{
    public:
    SerialResponseMessage::SerialResponseMessage(String _message);
    void sendMessage(SerialResponseMessage message_);
};



class Command
{
private:
    // byte message_id_;
    // char command_group_;
    // byte command_number_;
    String *message_;

public:
    // Command::Command(char _message_id, String *_message);
    Command::Command(String *_message);

    void executeCommand();
    void parseCommand();
};

String *checkForMessage();

#endif
