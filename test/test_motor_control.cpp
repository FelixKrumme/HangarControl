#include <unity.h>
#include "motor_control.h"

void test_setMicroStepConfig_valid(void) {
    StepperMotor motor(0, motor_group_plattform, front_left, 12, 13, 14, 2);

    // Test with a valid microstep configuration
    int valid_config = 2;
    int expected_steps_per_rotation = 400;
    motor.setMicroStepConfig(valid_config);
    TEST_ASSERT_EQUAL_INT(valid_config, motor.getMicroStepConfig());
    TEST_ASSERT_EQUAL_INT(expected_steps_per_rotation, motor.getStepsPerRotation());
}

void test_setMicroStepConfig_invalid(void) {
    StepperMotor motor(0, motor_group_plattform, front_left, 12, 13, 14, 4);


    // Test with an invalid microstep configuration
    int invalid_config = 3;
    motor.setMicroStepConfig(invalid_config);
    // Check that the microstep configuration was not set
    TEST_ASSERT_NOT_EQUAL(invalid_config, motor.getMicroStepConfig());
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_setMicroStepConfig_valid);
    RUN_TEST(test_setMicroStepConfig_invalid);
    UNITY_END();
}

void loop() {
    // Do nothing here
}