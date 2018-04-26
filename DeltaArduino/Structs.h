#ifndef STRUCTS_H
	
	#define STRUCTS_H

	#include "Defines.h"

	typedef struct CommandsBuffer { //Circular buffer for serial communication

		char command[MAX_COMMANDS_NUM][SERIAL_COMMAND_MAX_LEN];
		int command_len[MAX_COMMANDS_NUM];
		int start, end_;
		bool full, empty;

	}CommandsBuffer;


	//---------------------//

	typedef struct ServoInfo {


		int duty_cycle;
		int dc_offset;

		int max_duty_cycle;
		int min_duty_cycle;

		int mean_dc;

		int last_direction;
		int slack_compensation_val;

		char move_servo_from; //Change position with angles/microseconds/xyz

		int angle;
		
		//--ANGLE TO DUTY_CYCLE
		long m;
		long n;
		// float m;
		// float n;


	}ServoInfo;

	//---------------------//

	typedef struct DeltaInfo {

		float x;
		float y;
		float z;

		int speed;

		bool move_delta;

		int mode;

	}DeltaInfo;

#endif