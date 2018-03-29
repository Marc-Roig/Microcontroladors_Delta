#ifndef CONFIG_H
  
	#include <Servo.h>

	#include "Calibration.h"

	#define CONFIG_H

	//---------------//
	//------I/O------//
	//---------------//
	
	//SERVOS
	#define SERVO1_PIN 9
	#define SERVO2_PIN 10
	#define SERVO3_PIN 11
	#define SERVO4_PIN 5 //GRIPPER

	//CALIBRATION
	#define CHANGE_MODE_BUTTON_PIN 4
	#define INCREASE_DC_BUTTON_PIN 7
	#define DECREASE_DC_BUTTON_PIN 8
	#define CHANGE_STEP_CHANGE_PIN 12

	//----------------//
	//-----SERVOS-----//
	//----------------//
	#define MAX_DC_SERVO1 2300
	#define MAX_DC_SERVO2 2200
	#define MAX_DC_SERVO3 2200
	#define MAX_DC_SERVO4 1900 //GRIPPER

	#define MIN_DC_SERVO1 300
	#define MIN_DC_SERVO2 300
	#define MIN_DC_SERVO3 500
	#define MIN_DC_SERVO4 965 //GRIPPER

	#define SERVO1_COMPENSATION_VAL 100
	#define SERVO2_COMPENSATION_VAL 110
	#define SERVO3_COMPENSATION_VAL 0
	#define SERVO4_COMPENSATION_VAL 0

	#define SERVO1_M_ANGLE_TO_DC -10.478
	#define SERVO2_M_ANGLE_TO_DC -10.868
	#define SERVO3_M_ANGLE_TO_DC -8.41

	#define SERVO1_N_ANGLE_TO_DC 2542.5
	#define SERVO2_N_ANGLE_TO_DC 2335.7
	#define SERVO3_N_ANGLE_TO_DC 2389.3

	//-------------------//
	//----CALIBRATION----//
	//-------------------//

	//CHANGE DC MODE
	#define CHANGE_WITH_BUTTONS 0
	#define CHANGE_WITH_POTENTIOMETER 1
	#define CHANGE_WITH_SERIAL 2

	//SERVO MOVEMENT
	#define CLOCKWISE 0
	#define COUNTERCLOCKWISE 1


	//--------------//	
	//----SERIAL----//
	//--------------//	

	#define SERIAL_DELAY_MS 100

	//SERIAL COM MODES
	#define ASK_FOR_ANGLES  0
	#define GIVE_ANGLES  1

	//GCODE CODES

	#define MOVE_SERVOS 3
	#define CHANGE_SPEED 4
	#define MOVE_EF 5
	#define END_OF_STREAM 6
	#define EMERGENCY_STOP 9

	#define SEND_ME_ANGLES 20
	#define SEND_ANGLES_EFPOS 21

	//SERIAL BUFFER

	#define SERIAL_BUFFER_LEN 5
	#define SERIAL_COMMAND_MAX_LEN 16

	//---------------//
	//----STRUCTS----//
	//---------------//

	typedef struct Buffer { //Circular buffer, FIFO

		char command[SERIAL_BUFFER_LEN][SERIAL_COMMAND_MAX_LEN];
		int command_len[SERIAL_BUFFER_LEN];
		int start, end_;
		bool full, empty;

	}Buffer;

	typedef struct ServoInfo {

		int angle;

		int duty_cycle;

		int max_duty_cycle;
		int min_duty_cycle;

		int mean_dc;

		int last_direction;
		int slack_compensation_val;

		//--ANGLE TO DUTY_CYCLE
		int m;
		int n;

	}ServoInfo;

	//------------------------//
	//----GLOBAL VARIABLES----//
	//------------------------//
	Buffer buffer;

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
