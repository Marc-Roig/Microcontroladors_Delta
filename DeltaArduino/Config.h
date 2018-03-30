#ifndef CONFIG_H
  
	#include <Servo.h>

	#include "Defines.h"
	#include "Structs.h"

	#include "Calibration.h"

	#define CONFIG_H

	//------------------------//
	//----GLOBAL VARIABLES----//
	//------------------------//
	Buffer buffer;

	DeltaInfo deltainfo;

	Servo servos[4];
	ServoInfo servoinfo[4];

	int max_duty_cycles[4];
	int min_duty_cycles[4];

	int change_mode_button_pin;
	int increase_dc_button_pin;
	int decrease_dc_button_pin;
	int change_step_change_pin;

	int servo_direction[4];
	int change_dir_compensation_val[4];

#endif
