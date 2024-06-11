#include <unity.h>
#include "serial_communication.h"

void test_parse_valid_command(void) {
    CommandParser parser;
    // Test with a valid command
    TEST_ASSERT_TRUE(parser.parse("G1 G 2 S3 D4 V5"));
    TEST_ASSERT_TRUE(parser.parse("G1 2 R3 D4 V5"));
    TEST_ASSERT_TRUE(parser.parse("G1 2 R3 D4"));
    TEST_ASSERT_TRUE(parser.parse("G1 G 2 S543 D4"));
}

void test_parse_invalid_command(void) {
    CommandParser parser;
    // Test with an invalid command
    TEST_ASSERT_FALSE(parser.parse("H1 G 2 S3 D4 V5"));
    TEST_ASSERT_FALSE(parser.parse("G0 G 2 S3 D4 V5"));
    TEST_ASSERT_FALSE(parser.parse("G1 2 S3 D4 V5 6"));
}

void setup() {
    delay(2000); // Allow some time for the serial port to initialize
    UNITY_BEGIN();
    RUN_TEST(test_parse_valid_command);
    RUN_TEST(test_parse_invalid_command);
    UNITY_END();
}

void loop() {
    // Do nothing here
}