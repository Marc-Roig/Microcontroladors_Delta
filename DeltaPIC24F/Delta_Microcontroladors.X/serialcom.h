#ifndef SERIALCOM_H

	#define SERIALCOM_H

	void Serial_begin(int baudrate);
	void Serial_push_character(char incomingByte);
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