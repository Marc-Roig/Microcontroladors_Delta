#include <Servo.h>

Servo servo;

int max_duty_cycle = 2200;
int min_duty_cycle = 860;

int change_mode_button_pin = 9;
int increase_dc_button_pin = 10;
int decrease_dc_button_pin = 11;


void setup() {

	Serial.begin(9600);

	servo.attach(10);

	pinMode(change_mode_button_pin, INPUT);
	pinMode(increase_dc_button_pin, INPUT);
	pinMode(decrease_dc_button_pin, INPUT);
}

void loop() {

	servo.writeMicroseconds();

}

void potenciometer(int* duty_cycle) {

	*duty_cycle = map(analogRead(A0) ,0, 1023, min_duty_cycle, max_duty_cycle);

}

void polsadors(int* duty_cycle) {

    static int S3 = 0, S6 = 0, S4 = 0;
    static int mode = 0; 

    int step1 = 20, step2 = 40, step3 = 100;

    bool change_mode_button = digitalRead(change_mode_button_pin);
    bool increase_dc_button = digitalRead(increase_dc_button_pin);
    bool decrease_dc_button = digitalRead(decrease_dc_button_pin);
 
    //--INCREMENT duty_cycle--//
    if (increase_dc_button && S3 == 0) {

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

     S3 = 1;

    }
    else if (!decrease_dc_button) S3 = 0;

    //--DECREMENT duty_cycle--//
    if (decrease_dc_button && S6 == 0) {
     
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
     
     S6 = 1;

    }
    else if (!decrease_dc_button) S6 = 0;

    //--CHANGE STEP--//
    if (change_mode_button && S4 == 0) {

		mode = (mode + 1) % 3; 
		S4 = 1;

    }
    else if (!change_mode_button) S4 = 0;
}
