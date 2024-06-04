#include "serial_communication.h"

// byte SerialMessage::createMessageID()
// {
//     return random(0, 255);
// };

SerialMessage::SerialMessage(String _message)
{
    message_ = _message;
    // message_id_ = createMessageID();
}
String* checkForMessage()
{
    if (Serial.available() > 0)
    {
        String* _message = new String(Serial.readString());
        return _message;
    }
    return nullptr;
};

// Command::Command(byte _message_id, String *_message)
// {
//     message_id_ = _message_id;
//     message_ = _message;
// };

Command::Command(String *_message)
{
    message_ = _message;
};

void SerialMessage::sendMessage(String message)
{
    Serial.println(SerialMessage(message));
    return;
};