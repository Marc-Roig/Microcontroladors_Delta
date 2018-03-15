#ifndef SERIALCOM_H

	#define SERIALCOM_H

	//STRUCTS
	typedef struct Buffer { //Circular buffer, FIFO
		char command[SERIAL_BUFFER_LEN][SERIAL_COMMAND_MAX_LEN];
		int command_len[SERIAL_BUFFER_LEN];
		int start, end_;
		bool full, empty;
	}Buffer;


	void init_UART();

	void serial_push_character(char incomingByte);

	bool inc_buffer_end_pointer();
	bool inc_buffer_start_pointer();

	void init_buffer(Buffer* buffer);

	
	//GLOBAL VARIABLES
	extern volatile Buffer RX_buffer;
	extern volatile Buffer TX_buffer;
	extern volatile bool command_recieved;

#endif