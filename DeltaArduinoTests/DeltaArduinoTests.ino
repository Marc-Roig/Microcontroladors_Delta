#include <Servo.h>

#define CHANGE_WITH_BUTTONS 0
#define CHANGE_WITH_POTENCIOMETER 1

Servo servo;

int max_duty_cycle = 2400;
int min_duty_cycle = 600;

int change_mode_button_pin =  9;
int increase_dc_button_pin = 10;
int decrease_dc_button_pin = 11;
int change_step_change_pin = 12;


void setup() {

    Serial.begin(9600);

    servo.attach(6);

    pinMode(change_mode_button_pin, INPUT);
    pinMode(increase_dc_button_pin, INPUT);
    pinMode(decrease_dc_button_pin, INPUT);
    pinMode(change_step_change_pin, INPUT);

}

void loop() {

    static int duty_cycle = (min_duty_cycle + max_duty_cycle) / 2; // Start at the middle
    static int change_dc_mode = CHANGE_WITH_BUTTONS;

    switch (change_dc_mode) {

        case CHANGE_WITH_POTENCIOMETER:     potenciometer(&duty_cycle);
                                            break;

        case CHANGE_WITH_BUTTONS:           push_buttons(&duty_cycle);
                                            break;

    } 
    
    change_mode(&change_dc_mode);
    
    servo.writeMicroseconds(duty_cycle);

    serial_write_every_ms(1000, duty_cycle);
  
}

void serial_write_every_ms(int wait_time, int duty_cycle) {

    static unsigned long startMilis = millis();
    
    if ((millis() - startMilis) > wait_time) {
        startMilis = millis();
        Serial.println(duty_cycle);
    }

}

char* int_to_char(int number) {
    
    int number_len = 1;
    int temp = number;

    while(temp > 9) {
        temp /= 10;
        number_len++;
    }

    char* val = (char*) malloc(number_len+2);

    for (int i = 0; i < number_len; i++) {

        val[i] = number / (int)(pow(10, (number_len-i-1))) % 10 + '0';

    }
    
    val[number_len] = '\n';
    val[number_len+1] = '\0';

    return val;
}

void potenciometer(int* duty_cycle) {

    *duty_cycle = map(analogRead(A0) ,0, 1023, min_duty_cycle, max_duty_cycle);

}

void change_mode(int* change_dc_mode) {

    static int S4 = 0;
    bool change_dc_m_button = digitalRead(change_mode_button_pin);
    
    if (change_dc_m_button && !S4) {
    
      *change_dc_mode = (*change_dc_mode + 1) % 2;
      S4 = 1;
      
    }
    else if (!change_dc_m_button) S4 = 0;
}

void push_buttons(int* duty_cycle) {

    static int S1 = 0, S2 = 0, S3 = 0;
    static int mode = 0; 

    int step1 = 20, step2 = 40, step3 = 100;

    bool change_mode_button = digitalRead(change_step_change_pin);
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
    else if (!increase_dc_button) S1 = 0;

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

}
