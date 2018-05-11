#include "Config.h"

int move_axis_or_angles = JOYSTICK_MOVE_ANGLES;

void init_joystick()  {

    pinMode(JOYSTICK_BUTTON_PIN, INPUT);
    pinMode(JOYSTICK_CHANGE_AXIS_ANGLES, INPUT);
    pinMode(BUTTONS_STEP_XYZ_PIN, INPUT);
    pinMode(INCREASE_XYZ_BUTTON_PIN, INPUT);
    pinMode(DECREASE_XYZ_BUTTON_PIN, INPUT);

    pinMode(JOYSTICK_X_PIN, ANALOG_INPUT);
    pinMode(JOYSTICK_Y_PIN, ANALOG_INPUT);

    init_ADC();
    
    if (move_axis_or_angles == JOYSTICK_MOVE_ANGLES) {
        servoinfo[0].move_servo_from = MOVE_SERVO_FROM_ANGLE;
        servoinfo[1].move_servo_from = MOVE_SERVO_FROM_ANGLE;
        servoinfo[2].move_servo_from = MOVE_SERVO_FROM_ANGLE;
    }
    else {
        servoinfo[0].move_servo_from = MOVE_SERVO_FROM_XYZ;
        servoinfo[1].move_servo_from = MOVE_SERVO_FROM_XYZ;
        servoinfo[2].move_servo_from = MOVE_SERVO_FROM_XYZ;
    }
}

void joystick_movement() {

    switch (move_axis_or_angles) {

        case JOYSTICK_MOVE_ANGLES:      joystick_move_angles();
                                        break;

        case JOYSTICK_MOVE_AXIS:        joystick_move_xyz();
                                        // buttons_move_xyz();
                                        joystick2_move_gripper();
                                        break;

        default:                        break;

    }

    //joystick_debug();

    joysitck_change_mode(&move_axis_or_angles);

}

void joystick_debug() {

    static unsigned long StartTime = 0;

    int time_difference_ms = 500; // Every 400 ms the program will read the joystick val 

    if ((millis() - StartTime) > time_difference_ms ) {

        joystick_debug_from_angles();

        StartTime = millis();

    }

}

void joystick_debug_from_angles() {

    serial_write_angles();
//    serial_write_xyz_from_angles();
//    Serial_write("-----------------------\n");

}

void joystick_debug_from_xyz() {

}

void joystick_move_angles() {

    static unsigned long StartTime = 0;
    static int servo_num = 0;
    static int S0 = 0; //Flank detector

    int time_difference_ms = 400; // Every 200 ms program will read the joystick val 

    bool change_axis_button = digitalRead(JOYSTICK_BUTTON_PIN);
    
    if ((millis() - StartTime) > time_difference_ms ) {

        float joys_x = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -JOYSTICK_NUMBER_OF_SPEEDS/2, JOYSTICK_NUMBER_OF_SPEEDS/2);
        if (abs2(joys_x) < 1.5) joys_x = 0;

        servoinfo[servo_num].angle += (int)(0.5 * joys_x); //Multiply joys_x with a gain
        //Serial_println((int)joys_x);
        StartTime = millis();

    }

    //--CHANGE MOTOR--//
    if (!change_axis_button && !S0) {

        servo_num = (servo_num + 1) % 3;
        S0 = 1; 

    } else if (change_axis_button) S0 = 0;

}


/*********************************************************************
* Function: joysitck_change_mode(int* change_joystick_mode);
*
* Overview: Joystick can move angles or axis. Pressing the button
*           will change from moving angles to axis or viceversa.
*           
* PreCondition: none
*
* Input: int* change_joystick_mode  - mode of movement
*
* Output: none
*
********************************************************************/

void joysitck_change_mode(int* change_joystick_mode) {

    static int S0 = 0;
    bool joystick_change_mode_button = digitalRead(JOYSTICK_CHANGE_AXIS_ANGLES);

    if (joystick_change_mode_button && !S0) {

        *change_joystick_mode = (*change_joystick_mode + 1) % 2;

        switch (*change_joystick_mode) {

            case JOYSTICK_MOVE_ANGLES:      servoinfo[0].move_servo_from = MOVE_SERVO_FROM_ANGLE;
                                            servoinfo[1].move_servo_from = MOVE_SERVO_FROM_ANGLE;
                                            servoinfo[2].move_servo_from = MOVE_SERVO_FROM_ANGLE;
                                            break;

            case JOYSTICK_MOVE_AXIS:        servoinfo[0].move_servo_from = MOVE_SERVO_FROM_XYZ;
                                            servoinfo[1].move_servo_from = MOVE_SERVO_FROM_XYZ;
                                            servoinfo[2].move_servo_from = MOVE_SERVO_FROM_XYZ;
                                            break;
        }

        servoinfo[3].move_servo_from = MOVE_SERVO_FROM_DC;

        S0 = 1;

    }
    else if (!joystick_change_mode_button) S0 = 0;

}

void joystick2_move_gripper() {

    static unsigned long StartTime = 0;

    int time_difference_ms = 50; // Every X ms program will read the joystick val 

    if ((millis() - StartTime) > time_difference_ms ) {

        float joys_grip = map(analogRead(JOYSTICK_X_GRIPPER_PIN), 0, 1023, -7, 7);

        if (abs2(joys_grip) < 1.5) joys_grip = 0; // Minimum movement of joystick, to avoid jitter

        servoinfo[3].duty_cycle += (15 * joys_grip); //Multiply joys_grip with a gain
        if (servoinfo[3].duty_cycle > servoinfo[3].max_duty_cycle) servoinfo[3].duty_cycle = servoinfo[3].max_duty_cycle;
        if (servoinfo[3].duty_cycle < servoinfo[3].min_duty_cycle) servoinfo[3].duty_cycle = servoinfo[3].min_duty_cycle;

        StartTime = millis();

    }

}

/*********************************************************************
* Function: joystick_change_xy();
*
* Overview: Move X and Y of end effector with the joystick.
*           
* PreCondition: none
*
* Input: none
*
* Output: none
*
********************************************************************/

void joystick_move_xyz() {

    static char move_xy_or_z = 0; //0 for XY , 1 for Z;
    static char S0 = 0;

    bool change_axis_button = digitalRead(JOYSTICK_BUTTON_PIN);

    //--CHANGE XY Z--//
    if (!change_axis_button && !S0) {

        move_xy_or_z = (move_xy_or_z + 1) % 2;
        S0 = 1; 

    } else if (change_axis_button) S0 = 0;

    //--MOVING Z OR XY WITH JOYSITCK--//
    if (move_xy_or_z == 0) joystick_move_xy();
    else joystick_move_z();

}

void joystick_move_xy() {

    static unsigned long StartTime = 0;

    int time_difference_ms = 50; // Every 50 ms the program will read the joystick val 

    if ((millis() - StartTime) > time_difference_ms ) {

        float joys_x = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -JOYSTICK_NUMBER_OF_SPEEDS/2, JOYSTICK_NUMBER_OF_SPEEDS/2);
        float joys_y = map(analogRead(JOYSTICK_Y_PIN), 0, 1023, JOYSTICK_NUMBER_OF_SPEEDS/2, -JOYSTICK_NUMBER_OF_SPEEDS/2);

        if (abs2(joys_x) < 1.2) joys_x = 0;
        if (abs2(joys_y) < 1.2) joys_y = 0;

        float base_speed_increment = 0.5;

        deltainfo.x += joys_x * base_speed_increment;
        deltainfo.y += joys_y * base_speed_increment;


        StartTime = millis();
        
    } 
}

void joystick_move_z() {

    static unsigned long StartTime = 0;

    int time_difference_ms = 50; // Every X ms program will read the joystick val 

    if ((millis() - StartTime) > time_difference_ms ) {

        float joys_z = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -JOYSTICK_NUMBER_OF_SPEEDS/2, JOYSTICK_NUMBER_OF_SPEEDS/2);

        if (abs2(joys_z) < 1) joys_z = 0;

        deltainfo.z += (1 * joys_z); //Multiply joys_z with a gain


        StartTime = millis();

    }

}

/*********************************************************************
* Function: buttons_move_xyz();
*
* Overview: Move XYZ of end effector with the buttons.
*           
* PreCondition: none
*
* Input: none
*
* Output: none
*
********************************************************************/

void buttons_move_xyz() {

    static int S1 = 0, S2 = 0, S3 = 0, S4 = 0;//For flank detection
    static int step_val = 0; 
    static int axis = 0;

    int steps[] = {10, 50, 100}; //mm

    float xyz[] = {deltainfo.x, deltainfo.y, deltainfo.z};

    bool change_step_button = digitalRead(BUTTONS_STEP_XYZ_PIN);
    bool change_axis_button = digitalRead(JOYSTICK_BUTTON_PIN);
    bool increase_dc_button = digitalRead(INCREASE_XYZ_BUTTON_PIN);
    bool decrease_dc_button = digitalRead(DECREASE_XYZ_BUTTON_PIN);
 
    //--INCREMENT duty_cycle--//
    if (increase_dc_button && !S1) {

        xyz[axis] += steps[step_val];

        S1 = 1; 

    } else if (!increase_dc_button) S1 = 0;


    //--DECREMENT xyz[axis]--//
    if (decrease_dc_button && !S2) {
     
        xyz[axis] -= steps[step_val];

        S2 = 1;
     
    } else if (!decrease_dc_button) S2 = 0;


    //--CHANGE STEP--//
    if (change_step_button && !S3) {

        step_val = (step_val + 1) % 3; 
        S3 = 1;

    } else if (!change_step_button) S3 = 0;


    //--CHANGE AXIS--//
    if (change_axis_button && !S3) {

        axis = (axis + 1) % 3; 
        S4 = 1;

    } else if (!change_axis_button) S4 = 0;


    //--UPDATE VALUES--//
    deltainfo.x = xyz[0];
    deltainfo.y = xyz[1];
    deltainfo.z = xyz[2];

}
