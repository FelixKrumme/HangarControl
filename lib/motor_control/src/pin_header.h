#ifndef SWITCHES_PIN_HEADER_H
#define SWITCHES_PIN_HEADER_H

// Motor Pins
#define big_centring_front_dir_pin 1
#define big_centring_front_pul_pin 1
#define big_centring_back_dir_pin 1
#define big_centring_back_pul_pin 1

#define small_centring_dir_pin 1
#define small_centring_pul_pin 1

#define leveling_front_left_dir_pin 1
#define leveling_front_left_pul_pin 1
#define leveling_front_right_dir_pin 1
#define leveling_front_right_pul_pin 1
#define leveling_back_left_dir_pin 1
#define leveling_back_left_pul_pin 1
#define leveling_back_right_dir_pin 1
#define leveling_back_right_pul_pin 1

#define roof_dir_pin NULL
#define roof_pul_pin NULL

// Pin Chaange Interrupts usable Pins
// Arduino Uno/Nano/Mini: All pins are usable
// Arduino Mega: 10, 11, 12, 13, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64),
// A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)
// Interrupt Pins Mega, Mega2560, MegaADK
// 2, 3, 18, 19, 20, 21

// Endstop Pins
#define big_centring_front_endst_ind 10
#define big_centring_back_endst_ind 11
#define big_centring_endst_mec 12 // Two Mechanical Endstops used in series
// #define big_centring_front_endst_mec
// #define big_centring_back_endst_mec

#define small_centring_endst_ind 13
#define small_centring_endst_mec 50

#define leveling_front_left_endst_ind 51
#define leveling_front_right_endst_ind 52
#define leveling_back_left_endst_ind 53
#define leveling_back_right_endst_ind 62 // A8
#define leveling_endst_mec 63 // A9 // Four Mechanical Endstops used in series
// #define leveling_front_left_endst_mec 
// #define leveling_front_right_endst_mec 
// #define leveling_back_left_endst_mec 
// #define leveling_back_right_endst_mec 

#define roof_endst_ind NULL
#define roof_endst_mec NULL

// Switch Pins
#define switch_big_centring_left 1
#define switch_big_centring_right 1
#define switch_small_centring_left 1
#define switch_small_centring_right 1
#define switch_leveling_left 1
#define switch_leveling_right 1
#define roof_switch_left NULL
#define roof_switch_right NULL

#endif