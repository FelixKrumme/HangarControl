#ifndef SWITCHES_PIN_HEADER_H
#define SWITCHES_PIN_HEADER_H

// Motor Pins
#define big_centring_front_dir_pin 30
#define big_centring_front_pul_pin 31
#define big_centring_back_dir_pin 32
#define big_centring_back_pul_pin 33

#define small_centring_dir_pin 34
#define small_centring_pul_pin 35

#define leveling_front_left_dir_pin 22
#define leveling_front_left_pul_pin 23
#define leveling_front_right_dir_pin 24
#define leveling_front_right_pul_pin 25
#define leveling_back_left_dir_pin 26
#define leveling_back_left_pul_pin 27
#define leveling_back_right_dir_pin 28
#define leveling_back_right_pul_pin 29

#define roof_dir_pin NULL
#define roof_pul_pin NULL

// Pin Chaange Interrupts usable Pins
// Arduino Uno/Nano/Mini: All pins are usable
// Arduino Mega: 10, 11, 12, 13, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64),
// A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)
// Interrupt Pins Mega, Mega2560, MegaADK
// 2, 3, 18, 19, 20, 21

// Endstop Pins
// 50-53 used, 62, 63 used 
#define big_centring_front_endst_ind 62
#define big_centring_back_endst_ind 63
// #define big_centring_endst_mec 12 // Two Mechanical Endstops used in series
// #define big_centring_front_endst_mec
// #define big_centring_back_endst_mec

#define small_centring_endst_ind 52
// #define small_centring_endst_mec 50

#define leveling_front_left_endst_ind 53
#define leveling_front_right_endst_ind 51
#define leveling_back_left_endst_ind 50
#define leveling_back_right_endst_ind 12
// #define leveling_endst_mec  53//  // Four Mechanical Endstops used in series
// #define leveling_front_left_endst_mec 
// #define leveling_front_right_endst_mec 
// #define leveling_back_left_endst_mec 
// #define leveling_back_right_endst_mec 

#define roof_endst_ind NULL
#define roof_endst_mec NULL

// Switch Pins
#define switch_leveling_left 15
#define switch_leveling_right 14
#define switch_big_centring_left 19
#define switch_big_centring_right 18
#define switch_small_centring_left 17
#define switch_small_centring_right 16

#define roof_switch_left NULL // 20
#define roof_switch_right NULL // 21

#endif