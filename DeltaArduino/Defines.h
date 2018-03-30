#ifndef DEFINES_H

	#define DEFINES_H

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

	//ARMS
	#define MAX_DC_SERVO1 2300
	#define MAX_DC_SERVO2 2200
	#define MAX_DC_SERVO3 2200

	#define MIN_DC_SERVO1 300
	#define MIN_DC_SERVO2 300
	#define MIN_DC_SERVO3 500

	#define SERVO1_COMPENSATION_VAL 100
	#define SERVO2_COMPENSATION_VAL 110
	#define SERVO3_COMPENSATION_VAL 0

	#define SERVO1_M_ANGLE_TO_DC -10.478
	#define SERVO2_M_ANGLE_TO_DC -10.868
	#define SERVO3_M_ANGLE_TO_DC -8.41

	#define SERVO1_N_ANGLE_TO_DC 2542.5
	#define SERVO2_N_ANGLE_TO_DC 2335.7
	#define SERVO3_N_ANGLE_TO_DC 2389.3

	//GRIPPER
	#define MAX_DC_SERVO4 1900 
	#define MIN_DC_SERVO4 965 
	#define SERVO4_COMPENSATION_VAL 0

	//--------------------//
	//----DELTA PARAMS----//
	//--------------------//

	#define DELTA_MAX_Z 100
	#define DELTA_MIN_Z 200

	#define DELTA_START_X 0
	#define DELTA_START_Y 0
	#define DELTA_START_Z 0

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

#endif