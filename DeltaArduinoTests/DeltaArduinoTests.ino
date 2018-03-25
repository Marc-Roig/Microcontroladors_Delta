#include <Servo.h>

#define CHANGE_WITH_BUTTONS 0
#define CHANGE_WITH_POTENCIOMETER 1

Servo servo;

int max_duty_cycle = 2200;
int min_duty_cycle = 860;

int change_mode_button_pin = 09;
int increase_dc_button_pin = 10;
int decrease_dc_button_pin = 11;
int change_step_change_pin = 12;


void setup() {

	Serial.begin(9600);

	servo.attach(10);

	pinMode(change_mode_button_pin, INPUT);
	pinMode(increase_dc_button_pin, INPUT);
	pinMode(decrease_dc_button_pin, INPUT);
	pinMode(change_step_change_pin, INPUT);

}

void loop() {

	static int duty_cycle = (min_duty_cycle + max_duty_cycle) / 2; // Start at the middle
	static int change_dc_mode = CHANGE_WITH_POTENCIOMETER;

	switch (change_dc_mode) {

		case CHANGE_WITH_POTENCIOMETER:		potenciometer(&duty_cycle);
											break;

		case CHANGE_WITH_BUTTONS:			push_buttons(&duty_cycle, &change_dc_mode);
											break;

	} 

	servo.writeMicroseconds(duty_cycle);

	serial_write_every_ms(500, int_to_char(duty_cycle));

}

void serial_write_every_ms(int wait_time, char duty_cycle[]) {

	static unsigned long startMilis = millis();

	if ((millis() - startMilis) > wait_time) {
		startMilis = millis();
		Serial.write(duty_cycle);
	}

}

char* int_to_char(int number) {
	
	int number_len = 1;
	int temp = number;

	while(temp > 9) {
		temp /= 10;
		number_len++;
	}

	char* val = (char*) malloc(number_len);

	for (int i = 0; i < number_len; i++) {

		val[i] = number / (int)(pow(10, (number_len-i-1))) % 10 + '0';

	}

	char* result = val;
	free(val); // Dont know if program free's it when exits function

	return result;
}



void potenciometer(int* duty_cycle) {

	*duty_cycle = map(analogRead(A0) ,0, 1023, min_duty_cycle, max_duty_cycle);

}

void push_buttons(int* duty_cycle, int* change_dc_mode) {

    static int S1 = 0, S2 = 0, S3 = 0; S4 = 0;
    static int mode = 0; 

    int step1 = 20, step2 = 40, step3 = 100;

    bool change_mode_button = digitalRead(change_mode_button_pin);
    bool change_dc_m_button = digitalRead(change_mode_button_pin);
    bool increase_dc_button = digitalRead(increase_dc_button_pin);
    bool decrease_dc_button = digitalRead(decrease_dc_button_pin);
 
    //--INCREMENT duty_cycle--//
    if (increase_dc_button && !S1) {

     switch(mode) {
        case 0:
            *duty_cycle = *duty_cycle + step1;
            break;
        case 1:
            *duty_cycle = *duty_cycle + step2;
            break;
        default:
            *duty_cycle = *duty_cycle + step3;
            break;
     }

     if (*duty_cycle >= max_duty_cycle) *duty_cycle = max_duty_cycle;

     S1 = 1;
    }
    else if (!decrease_dc_button) S1 = 0;

    //--DECREMENT duty_cycle--//
    if (decrease_dc_button && !S2) {
     
     switch(mode) {
        case 0:
            *duty_cycle = *duty_cycle - step1;
            break;
        case 1:
            *duty_cycle = *duty_cycle - step2;
            break;
        default:
            *duty_cycle = *duty_cycle - step3;
            break;
     }

     if (*duty_cycle <= min_duty_cycle) *duty_cycle = min_duty_cycle;

     S2 = 1;

    }
    else if (!decrease_dc_button) S2 = 0;

    //--CHANGE STEP--//
    if (change_mode_button && !S3) {

		mode = (mode + 1) % 3; 
		S3 = 1;

    }
    else if (!change_mode_button) S3 = 0;

    if (change_dc_m_button && !S4)

}
