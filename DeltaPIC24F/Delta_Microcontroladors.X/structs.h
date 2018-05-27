#ifndef STRUCTS_H
	#define STRUCTS_H
	
	typedef struct CommandsBuffer { //Circular buffer, FIFO
		int command_len[MAX_COMMANDS_NUM];
		int start, end_;
		bool full, empty;
		char command[MAX_COMMANDS_NUM][SERIAL_COMMAND_MAX_LEN];
	}CommandsBuffer;

	typedef struct SerialBuffer { //Circular buffer, FIFO
		int start, end_;
		bool full, empty;
		char command[SERIAL_BUFFER_LEN];
	}SerialBuffer;



	typedef struct DeltaInfo {

		float x;
		float y;
		float z;

		int speed;

		bool move_delta;

		int mode;

	}DeltaInfo;


	
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
        
        long m;
        long n;

    }ServoInfo;

#endif