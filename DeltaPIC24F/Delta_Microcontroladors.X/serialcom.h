#ifndef SERIALCOM_H

	#define SERIALCOM_H

	//STRUCTS
	typedef struct Buffer { //Circular buffer, FIFO
		char command[SERIAL_BUFFER_LEN][SERIAL_COMMAND_MAX_LEN];
		int command_len[SERIAL_BUFFER_LEN];
		int start, end_;
		bool full, empty;
	}Buffer;

	typedef struct TXBuffer { //Circular buffer, FIFO
		char command[TX_SERIAL_BUFFER_LEN];
		int start, end_;
		bool full, empty;
	}TXBuffer;


	void init_UART();

	void serial_write(char data_to_print[]);
	void serial_println(int value);
	void serial_print(int value);

	void parse_command(char command[SERIAL_COMMAND_MAX_LEN]);
	void serial_next_instruction();
	void send_command_header(int command_num, bool end_with_new_line);

	void serial_send_angles();

	void serial_push_character(char incomingByte);

	void serial_recieve_angles(char command[SERIAL_COMMAND_MAX_LEN]);

	void init_buffer(Buffer* buffer);
	void init_TXbuffer();

	bool inc_buffer_end_pointer(struct Buffer* buffer);
	bool inc_TXbuffer_end_pointer();
	bool inc_buffer_start_pointer(struct Buffer* buffer);
	bool inc_TXbuffer_start_pointer();

	char* int_to_char_2digits(int numb);
	char* int_to_char_3digits(int numb);
	char* int_to_char(int number);
	int strlength(char *p);
	int chars_to_int(char a, char b, char c);
	bool is_alphanumeric(char a);
	
	//GLOBAL VARIABLES
	extern volatile Buffer RX_buffer;
	extern volatile TXBuffer TX_buffer;

	extern volatile bool Serial_busy;

	extern volatile bool command_recieved;
    
    extern volatile int serial_mode;

#endif