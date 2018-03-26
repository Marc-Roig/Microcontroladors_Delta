//--CHANGE DC MODE--//
#define CHANGE_WITH_BUTTONS 0
#define CHANGE_WITH_POTENCIOMETER 1

//--SERVO MOVEMENT--//
#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1

//If these values are changed servos need to be calibrated again
int max_duty_cycle = 2130;
int min_duty_cycle = 300;

int change_mode_button_pin =  9;
int increase_dc_button_pin = 10;
int decrease_dc_button_pin = 11;
int change_step_change_pin = 12;

//Every time the servo turns it has to compensate the slack of it 
//Track of the previous direction is needed
int servo_direction = CLOCKWISE;
int change_dir_compensation_val = 20;

calibration_initial_positions(bool move_servo1, bool move_servo2, bool move_servo3) {

	bool move_servos[] = {move_servo1, move_servo2, move_servo3};

	int mean_dc[3];
	mean_dc[0] = (max_duty_cycle + min_duty_cycle) / 2;

	for (int i = 0; i < 3; i++) {
		
	    servo[i].writeMicroseconds(mean_dc[0] + 500);
	    delay(500);
	    servo[i].writeMicroseconds(mean_dc[0] - 500);

	}

}