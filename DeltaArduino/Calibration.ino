//--CHANGE DC MODE--//
#define CHANGE_WITH_BUTTONS 0
#define CHANGE_WITH_POTENTIOMETER 1

//--SERVO MOVEMENT--//
#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1


int max_duty_cycles[] = {2130, 2130, 2130};
int min_duty_cycles[] = {300, 300, 300};

int change_mode_button_pin =  9;
int increase_dc_button_pin = 10;
int decrease_dc_button_pin = 11;
int change_step_change_pin = 12;

//Every time the servo turns it has to compensate the slack of it 
//Track of the previous direction is needed
int servo_direction = CLOCKWISE;
int change_dir_compensation_val = 20;


void servo_calibration(bool move_servo1, bool move_servo2, bool move_servo3) {

    static int duty_cycle[3] = { (min_duty_cycle[0] + max_duty_cycle[0]) / 2 - 500,     // Start at the middle
                                 (min_duty_cycle[1] + max_duty_cycle[1]) / 2 - 500,
                                 (min_duty_cycle[2] + max_duty_cycle[2]) / 2 - 500 }

    static int change_dc_mode = CHANGE_WITH_BUTTONS;

    switch (change_dc_mode) {

        case CHANGE_WITH_POTENTIOMETER:     if (move_servo1) potenciometer(&duty_cycle[0]);
                                            if (move_servo2) potenciometer(&duty_cycle[1]);
                                            if (move_servo3) potenciometer(&duty_cycle[2]);
                                            break;

        case CHANGE_WITH_BUTTONS:           if (move_servo1) push_buttons(&duty_cycle[0]);
                                            if (move_servo2) push_buttons(&duty_cycle[1]);
                                            if (move_servo3) push_buttons(&duty_cycle[2]);
                                            break;

        default:                            break;

    } 
    
    calibration_change_dc_mode(&change_dc_mode);
    
    if (move_servo1) servo[0].writeMicroseconds(duty_cycle);
    if (move_servo2) servo[1].writeMicroseconds(duty_cycle);
    if (move_servo3) servo[2].writeMicroseconds(duty_cycle);

    // serial_write_every_ms(2000, duty_cycle);

}

/*********************************************************************
* Function: void calibration_initial_positions(bool move_servo1, bool move_servo2, bool move_servo3);
*
* Overview: To reduce the slack of the gears all servos are moved
*           clockwise. To ensure they are left moved clockwise
*           they are first moved counterclockwise just in case they
*           coudln't move any more clockwise.
*
* PreCondition: none
*
* Input: bool - Will move the servo 1
*        bool - Will move the servo 2
*        bool - Will move the servo 3
*
* Output: none
*
********************************************************************/

void calibration_initial_positions(bool move_servo1, bool move_servo2, bool move_servo3) {

    bool move_servos[] = {move_servo1, move_servo2, move_servo3};

    int mean_dc[3];
    for (int i = 0; i < 3; i++) {
        mean_dc[i] = (max_duty_cycles[i] + min_duty_cycles[i]) / 2;
    }

    for (int i = 0; i < 3; i++) {
        
        servo[i].writeMicroseconds(mean_dc[i] + 500);
        delay(500);
        servo[i].writeMicroseconds(mean_dc[i] - 500);

    }

}


void calibration_change_dc_potentiometer(int* duty_cycle) {

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

void calibration_change_dc_mode(int* change_dc_mode) {

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