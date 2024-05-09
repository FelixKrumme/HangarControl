class MotorDefinition
{
private:
    int motor_group;
    int motor_position;
    int motor_dir_pin;
    int motor_step_pin;
};

class MotorGroup
{
    private:
    int group_id;
    int group_speed;
};

turnMotorGroupOn(MotorGroup motor_group){
    //aktiviert den motor in schleife

}

turnMotor(MotorDefinition motor){
    setPulHigh
    setDirHigh
    // führt für 100 schritte ne bewegung aus
}





// Use this Enumerators for assinging a Motor to a Group
enum motor_group_id{motor_group_plattform, motor_group_centring, motor_group_roof};

