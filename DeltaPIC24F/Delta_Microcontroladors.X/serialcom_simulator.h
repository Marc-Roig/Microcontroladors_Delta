#ifndef SERIALCOM_SIMULATOR
	
	#define SERIALCOM_SIMULATOR
	
	void serial_com_with_simulator();
	void parse_command(char command[SERIAL_COMMAND_MAX_LEN]);
	void serial_next_instruction();
	void check_serial();
	bool inc_buffer_end_pointer();
	bool inc_buffer_start_pointer();
	void init_buffer();
	void send_command_header(int command_num, bool end_with_new_line);
	void serial_send_angles();
	void serial_recieve_angles(char command[SERIAL_COMMAND_MAX_LEN]);
	char* int_to_char_2digits(int numb);
	char* int_to_char_3digits(int numb);
	double power(double base, double expon);
	void int_to_char(int number, char* converted_char);
	int strlength(char *p);
	int chars_to_int(char a, char b, char c);
	bool is_alphanumeric(char a);

#endif