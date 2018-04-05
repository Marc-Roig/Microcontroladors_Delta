#ifndef STRUCTS_H
	
	#define STRUCTS_H

	#include "Defines.h"

	typedef struct Buffer { //Circular buffer for serial communication

		char command[SERIAL_BUFFER_LEN][SERIAL_COMMAND_MAX_LEN];
		int command_len[SERIAL_BUFFER_LEN];
		int start, end_;
		bool full, empty;

	}Buffer;



	//---------------------//

	typedef struct ServoInfo {

		float angle;

		int duty_cycle;
		int dc_offset;

		int max_duty_cycle;
		int min_duty_cycle;

		int mean_dc;

		int last_direction;
		int slack_compensation_val;

		//--ANGLE TO DUTY_CYCLE
		float m;
		float n;

		char* move_servo_from; //Change position with angles/microseconds/xyz

	}ServoInfo;

	//---------------------//

	typedef struct DeltaInfo {

		float x;
		float y;
		float z;

		int speed;

		bool move_delta;

	}DeltaInfo;

#endif