#ifndef SERIALCOM_H

	#define SERIALCOM_H

	//STRUCTS
	typedef struct Buffer { //Circular buffer, FIFO
		int command_len[SERIAL_BUFFER_LEN];
		int start, end_;
		bool full, empty;
		char command[SERIAL_BUFFER_LEN][SERIAL_COMMAND_MAX_LEN];
	}Buffer;

	typedef struct SerialBuffer { //Circular buffer, FIFO
		int start, end_;
		bool full, empty;
		char command[SERIAL_BUFFER_LEN];
	}SerialBuffer;


	void Serial_begin(int baudrate);
	void serial_push_character(char incomingByte);
	void Serial_write(char data_to_print[]);
	char Serial_read();
	int Serial_available();
	void Serial_println(int value);
	void Serial_print(int value);
	void init_RXbuffer();
	void init_TXbuffer();
	bool inc_RXbuffer_end_pointer();
	bool inc_TXbuffer_end_pointer();
	bool inc_RXbuffer_start_pointer();
	bool inc_TXbuffer_start_pointer();
	
	//GLOBAL VARIABLES
	extern volatile SerialBuffer RX_buffer;
	extern volatile SerialBuffer TX_buffer;

	extern volatile bool Serial_busy;

#endif