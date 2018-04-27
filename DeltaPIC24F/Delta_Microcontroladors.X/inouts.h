#ifndef INOUTS_H

	#define INOUTS_H
	
	#define OUTPUT 0
	#define INPUT 1
	#define ANALOG_INPUT 2

	#define HIGH 1
	#define LOW 0

	#define IO_RB3 0
	#define IO_RB8 1
	#define IO_RB9 2
	#define IO_RE8 3
	#define IO_RE9 4
	#define IO_RA14 5
	#define IO_RA15 6

	#define MS_BETWEEN_ADC_CONVERSION 50

	void pinMode(int pin_name, int mode);
	void init_ADC();
	void init_analog_input(int pin_name);
	void ADC_update_values();
	int analogRead(int pin_num);
	void digitalWrite(int pin_num, int value);
	int digitalRead(int pin_num);
	long map(long x, long in_min, long in_max, long out_min, long out_max);
	float abs2(float val);

#endif