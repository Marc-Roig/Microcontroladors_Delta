#include "Config.h"

/*********************************************************************
* Function: void servo_calibration(bool move_servo1, bool move_servo2, bool move_servo3);
*
* Overview: Reads the buttons and potentiometer
*           and then moves the servos
*
* PreCondition: Needs to be called in the main loop
*
* Input: bool - Will move the servo 1
*        bool - Will move the servo 2
*        bool - Will move the servo 3
*
* Output: none
*
********************************************************************/

void servo_calibration(bool move_servo1, bool move_servo2, bool move_servo3, bool move_servo4) {

    static int change_dc_mode = CHANGE_WITH_POTENTIOMETER;

    switch (change_dc_mode) {

        case CHANGE_WITH_POTENTIOMETER:     if (move_servo1) calibration_change_dc_potentiometer(0);
                                            if (move_servo2) calibration_change_dc_potentiometer(1);
                                            if (move_servo3) calibration_change_dc_potentiometer(2);
                                            if (move_servo4) calibration_change_dc_potentiometer(3);
                                            break;

        case CHANGE_WITH_BUTTONS:           if (move_servo1) calibration_change_dc_buttons(0);
                                            if (move_servo2) calibration_change_dc_buttons(1);
                                            if (move_servo3) calibration_change_dc_buttons(2);
                                            if (move_servo4) calibration_change_dc_buttons(3);
                                            break;

        default:                            break;

    } 
    
    calibration_change_dc_mode(&change_dc_mode);    

    debug_calibration(1000);

}

void debug_calibration(int wait_time) {
    
    static unsigned int startMilis = 0;
    
    if ((millis() - startMilis) > wait_time) {

        startMilis = millis();
        // serial_write_xyz_from_anlges();
        serial_write_dc_every_ms(0);
        // serial_write_angles();
        // Serial.write("-----------------------\n");
    }
}


/*********************************************************************
* Function: void calibration_start(bool move_servo1, bool move_servo2, bool move_servo3);
*
* Overview: Prepare board to calibrate servos
*
* PreCondition: Needs to be called in the setup function
*
* Input: bool - Will move the servo 1
*        bool - Will move the servo 2
*        bool - Will move the servo 3
*
* Output: none
*
********************************************************************/

void calibration_start(bool move_servo1, bool move_servo2, bool move_servo3, bool move_servo4) {

    pinMode(CHANGE_MODE_BUTTON_PIN, INPUT);
    pinMode(INCREASE_DC_BUTTON_PIN, INPUT);
    pinMode(DECREASE_DC_BUTTON_PIN, INPUT);
    pinMode(CHANGE_STEP_CHANGE_PIN, INPUT);

    pinMode(CALIBRATION_POTE_PIN, ANALOG_INPUT);

    init_ADC();

    servoinfo[0].move_servo_from = MOVE_SERVO_FROM_DC;
    servoinfo[1].move_servo_from = MOVE_SERVO_FROM_DC;
    servoinfo[2].move_servo_from = MOVE_SERVO_FROM_DC;
    servoinfo[3].move_servo_from = MOVE_SERVO_FROM_DC;

}


/*********************************************************************
* Function: void calibration_change_dc_mode(int* change_dc_mode);
*
* Overview: When the change mode button is pressed the duty
*           cycle changes from the potentiometer to the
*           buttons or visce versa
*
* PreCondition: All servoinfo values have to be set
*
* Input: int* - actual duty cycle change mode
*
* Output: none
*
********************************************************************/

void calibration_change_dc_mode(int* change_dc_mode) {

    static int S4 = 0;
    bool change_dc_m_button = digitalRead(CHANGE_MODE_BUTTON_PIN);
    
    if (change_dc_m_button && !S4) {
    
      *change_dc_mode = (*change_dc_mode + 1) % 2;
      S4 = 1;
      
    }
    else if (!change_dc_m_button) S4 = 0;

}


/*********************************************************************
* Function: void calibration_change_dc_potentiometer(int servo_num);
*
* Overview: Change duty cycle moving the potentiometer
*
* PreCondition: All servoinfo values have to be set
*
* Input: int - number of servo to move
*
* Output: none
*
********************************************************************/

void calibration_change_dc_potentiometer(int servo_num) {

    int min_dc = servoinfo[servo_num].min_duty_cycle;
    int max_dc = servoinfo[servo_num].max_duty_cycle;

    int duty_cycle = map(analogRead(CALIBRATION_POTE_PIN) ,0 , 1023, min_dc, max_dc);

    // Serial.println(duty_cycle);

    check_servo_change_direction(servo_num, duty_cycle);

    servoinfo[servo_num].duty_cycle = duty_cycle;

}


/*********************************************************************
* Function: void calibration_change_dc_buttons(int servo_num);
*
* Overview: Change duty cycle pushing buttons
*
* PreCondition: All servoinfo values have to be set
*
* Input: int - number of servo to move
*
* Output: none
*
********************************************************************/

void calibration_change_dc_buttons(int servo_num) {

    static int S1 = 0, S2 = 0, S3 = 0;
    static int step_val = 0; 

    int steps[] = {20, 40, 100};
    int duty_cycle = servoinfo[servo_num].duty_cycle;

    bool change_step_button = digitalRead(CHANGE_STEP_CHANGE_PIN);
    bool increase_dc_button = digitalRead(INCREASE_DC_BUTTON_PIN);
    bool decrease_dc_button = digitalRead(DECREASE_DC_BUTTON_PIN);
 
    //--INCREMENT duty_cycle--//
    if (increase_dc_button && !S1) {

        //Increase value depending on the step_val
        duty_cycle += steps[step_val];

        //Limit max angle
        if (duty_cycle >= servoinfo[servo_num].max_duty_cycle) duty_cycle = servoinfo[servo_num].max_duty_cycle;

        //Flank detection
        S1 = 1;

        //Change direction compensation
        if (servoinfo[servo_num].last_direction == CLOCKWISE) {
            servoinfo[servo_num].last_direction = COUNTERCLOCKWISE;
            duty_cycle += servoinfo[servo_num].slack_compensation_val;
        }

    }
    else if (!increase_dc_button) S1 = 0;

    //--DECREMENT duty_cycle--//
    if (decrease_dc_button && !S2) {

        duty_cycle -= steps[step_val];

        if (duty_cycle <= servoinfo[servo_num].min_duty_cycle) duty_cycle = servoinfo[servo_num].min_duty_cycle;

        S2 = 1;

        if (servoinfo[servo_num].last_direction == COUNTERCLOCKWISE) {

            servoinfo[servo_num].last_direction = CLOCKWISE;
            duty_cycle -= servoinfo[servo_num].slack_compensation_val;

        }

    }
    else if (!decrease_dc_button) S2 = 0;

    //--CHANGE STEP--//
    if (change_step_button && !S3) {

        step_val = (step_val + 1) % 3; 
        S3 = 1;

    }
    else if (!change_step_button) S3 = 0;

    servoinfo[servo_num].duty_cycle = duty_cycle;   //Store the value in the global variable

}
