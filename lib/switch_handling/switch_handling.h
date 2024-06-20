/**
 * @file switch_handling.h
 * @brief \todo
 * 
 * \todo
 * 
 * @author \todo
 * @date \todo
*/

#ifndef SWITCH_HANDLING
#define SWITCH_HANDLING

#include <Arduino.h>

// Handle Switches
// There are 4 Switches and several endstops (the final count is yet to be decided on).
// The switches should manually trigger a movement.
// The endstops should stop the movement with no regards how the action started.
// 
// If a movement e.g. for the levelling of the platform is started, by a Serial Command, we need to register ISRs for all Endstops belonging to the Movement.
//
// If a switch is used ISRs should be registered for the endstops as well as for the Switch itself.
// Switch 1: Leveling Platform
// Switch 2: Centring Platform x-wise
// Switch 3: Centring Platform y-wise
// Switch 4: Control of the Roof (not implemented yet)
// The switches are "real switches" and not buttons.


#endif
