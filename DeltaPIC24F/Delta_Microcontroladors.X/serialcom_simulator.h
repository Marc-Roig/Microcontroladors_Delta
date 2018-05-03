#ifndef SERIALCOM_SIMULATOR
	
	#define SERIALCOM_SIMULATOR
	
	void serial_com_with_simulator();
	void parse_command(char command[SERIAL_COMMAND_MAX_LEN]);
	void serial_next_instruction();
	void check_serial();
	bool inc_buffer_end_pointer();
	bool inc_buffer_start_pointer();
	void init_buffer();
	void serial_recieve_angles(char command[SERIAL_COMMAND_MAX_LEN]);
	void serial_recieve_ef_pos(char command[SERIAL_COMMAND_MAX_LEN]);
	void serial_send_angles();
	void serial_send_dc();
	void serial_send_speed();
	void serial_send_ef_pos();
	void send_command_header(int command_num, bool end_with_new_line);
	void serial_write_dc_every_ms(int wait_time);
	void serial_write_angles();

#endif