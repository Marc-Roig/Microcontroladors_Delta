#include "Config.h"

void joystick_change_xy() {

	// int joystick_number_of_speeds = 10;

	float joys_x = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -JOYSTICK_NUMBER_OF_SPEEDS/2, JOYSTICK_NUMBER_OF_SPEEDS/2);
	float joys_y = map(analogRead(JOYSTICK_Y_PIN), 0, 1023, -JOYSTICK_NUMBER_OF_SPEEDS/2, JOYSTICK_NUMBER_OF_SPEEDS/2);

	float base_speed_increment = 0.1;

	deltainfo.x += joys_x * base_speed; 
	deltainfo.y += joys_y * base_speed; 

}

void buttons_change_z() {
	
}