#include <Servo.h>

//--CHANGE DC MODE--//
#define CHANGE_WITH_BUTTONS 0
#define CHANGE_WITH_POTENTIOMETER 1

//--SERVO MOVEMENT--//
#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1

Servo servo;

//If these values are changed servos need to be calibrated again
int max_duty_cycle = 2130;
int min_duty_cycle = 300;

int change_mode_button_pin =  9;
int increase_dc_button_pin = 10;
int decrease_dc_button_pin = 11;
int change_step_change_pin = 12;

//Every time the servo turns it has to compensate the slack of it 
//Track of the previous direction is needed
int servo_direction = CLOCKWISE;
int change_dir_compensation_val = 20;

//CALIBRATION :
    //READ THE CURRENT POSITION WITH THE POT. VALUE
    //LOOK FOR THE CENTER WITH THE POT. VALUE AND MOVE
    //CHANGE VARIABLE SERVODIRECTION DEPENDING ON THE TRAJECTORY IT TOOK
//SYSTEM HAS TO BE ABLE TO COMPENSATE OFFSETS WITH THE POT. VALUE FEEDBACK

void setup() {

    Serial.begin(9600);

    servo.attach(6);

    pinMode(change_mode_button_pin, INPUT);
    pinMode(increase_dc_button_pin, INPUT);
    pinMode(decrease_dc_button_pin, INPUT);
    pinMode(change_step_change_pin, INPUT);

    //THE FIRST MOVE HAS TO LEFT THE SERVO MOVED CLOCKWISED (SLACK CORRECTION)
    int mean_dc = (max_duty_cycle + min_duty_cycle) / 2;
    servo.writeMicroseconds(mean_dc + 500);
    delay(500);
    servo.writeMicroseconds(mean_dc - 500);

}

void loop() {

    static int duty_cycle = (min_duty_cycle + max_duty_cycle) / 2 - 500; // Start at the middle
    static int change_dc_mode = CHANGE_WITH_BUTTONS;

    switch (change_dc_mode) {

        case CHANGE_WITH_POTENTIOMETER:     potentiometer(&duty_cycle);
                                            break;

        case CHANGE_WITH_BUTTONS:           push_buttons(&duty_cycle);
                                            break;

    } 
    
    change_mode(&change_dc_mode);
    
    servo.writeMicroseconds(duty_cycle);

    serial_write_every_ms(2000, duty_cycle);
  
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

void potentiometer(int* duty_cycle) {

    static int previous_dc = *duty_cycle; //

    *duty_cycle = map(analogRead(A0) ,0 , 1023, min_duty_cycle, max_duty_cycle);

    int difference = *duty_cycle - previous_dc; 
    int min_differnece = 50; //Min value to apply the compensation, to avoit jitter


    if (difference < -min_differnece) {
        if (servo_direction == CLOCKWISE) {
            servo_direction = COUNTERCLOCKWISE;
            *duty_cycle -= change_dir_compensation_val;
        }
        previous_dc = *duty_cycle; 

    }
    else if (difference > min_differnece) {
        if (servo_direction == COUNTERCLOCKWISE) {
            servo_direction = CLOCKWISE;
            *duty_cycle += change_dir_compensation_val;
        }   
        previous_dc = *duty_cycle;

    } //Do not refresh previous_dc it the change of dc has not surpassed the min_difference

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

        //Increase value depending on the mode
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

        //Limit max angle
        if (*duty_cycle >= max_duty_cycle) *duty_cycle = max_duty_cycle;

        //Flank detection
        S1 = 1;

        //Change direction compensation
        if (servo_direction == CLOCKWISE) {
            servo_direction = COUNTERCLOCKWISE;
            *duty_cycle += change_dir_compensation_val;
        }

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

     if (servo_direction == COUNTERCLOCKWISE) {
        servo_direction = CLOCKWISE;
        *duty_cycle -= change_dir_compensation_val;
     }

    }
    else if (!decrease_dc_button) S2 = 0;

    //--CHANGE STEP--//
    if (change_mode_button && !S3) {

        mode = (mode + 1) % 3; 
        S3 = 1;

    }
    else if (!change_mode_button) S3 = 0;

}
