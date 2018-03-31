#include "Config.h"

/*********************************************************************
* Function: joystick_change_xy();
*
* Overview: Move X and Y of end effector with the joystick.
*			Speed of end effector can be calculated like this:
*			X_speed = (joys_x * base_speed) / time_difference_ms  
* 			In the end the speed that will matter be each servo's speed.
* 			
* PreCondition: none
*
* Input: none
*
* Output: none
*
********************************************************************/

void joystick_change_xy() {

	static int StartTime = millis();

	int time_difference_ms = 200; // Every 200 ms program will read the joystick val 
	// int joystick_number_of_speeds = 10;

	if ((millis() - StartTime) > time_difference_ms ) {

		float joys_x = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -JOYSTICK_NUMBER_OF_SPEEDS/2, JOYSTICK_NUMBER_OF_SPEEDS/2);
		float joys_y = map(analogRead(JOYSTICK_Y_PIN), 0, 1023, -JOYSTICK_NUMBER_OF_SPEEDS/2, JOYSTICK_NUMBER_OF_SPEEDS/2);

		float base_speed_increment = 0.1;

		deltainfo.x += joys_x * base_speed_increment; 
		deltainfo.y += joys_y * base_speed_increment; 

		StartTime = millis();
	}

}

void buttons_change_z() {
	
}