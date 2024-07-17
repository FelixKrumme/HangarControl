#include <Arduino.h>
#include <unity.h>
#include "main.cpp" // Assuming processIncomingCommand is declared here

// Develop this test file further for testing of the Serial COmmand Receiving and Processing
// Mock functions
bool movePlatformUpCalled = false;
bool movePlatformDownCalled = false;

void movePlatformUp(StepperGroup& group) {
    movePlatformUpCalled = true;
}

void movePlatformDown(StepperGroup& group) {
    movePlatformDownCalled = true;
}

void setUp(void) {
    // Setup code here, run before each test
    movePlatformUpCalled = false;
    movePlatformDownCalled = false;
}

void tearDown(void) {
    // Teardown code here, run after each test
}

void test_processIncomingCommand_LevelingUp(void) {
    processIncomingCommand("ML1");
    TEST_ASSERT_TRUE(movePlatformUpCalled);
    TEST_ASSERT_FALSE(movePlatformDownCalled);
}

void test_processIncomingCommand_LevelingDown(void) {
    processIncomingCommand("ML0");
    TEST_ASSERT_TRUE(movePlatformDownCalled);
    TEST_ASSERT_FALSE(movePlatformUpCalled);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_processIncomingCommand_LevelingUp);
    RUN_TEST(test_processIncomingCommand_LevelingDown);
    UNITY_END();
}

void loop() {
    // Not used in this test
}