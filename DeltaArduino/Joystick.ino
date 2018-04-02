#include "Config.h"

void init_joystick()  {

    pinMode(JOYSTICK_BUTTON_PIN, INPUT);
    pinMode(JOYSTICK_CHANGE_AXIS_ANGLES, INPUT);
    pinMode(BUTTONS_STEP_XYZ_PIN, INPUT);
    pinMode(INCREASE_XYZ_BUTTON_PIN, INPUT);
    pinMode(DECREASE_XYZ_BUTTON_PIN, INPUT);

}

void joystick_movement() {

    static int move_axis_or_angles = JOYSTICK_MOVE_ANGLES;

    switch (move_axis_or_angles) {

        case JOYSTICK_MOVE_ANGLES:      joystick_change_angles();
                                        break;

        case JOYSTICK_MOVE_AXIS:        joystick_change_xy();
                                        buttons_change_xyz();
                                        convert_xyz_to_angles();
                                        break;

        default:                        break;

    }

    joysitck_change_mode(&move_axis_or_angles);

}

void joystick_change_angles() {

    static unsigned long StartTime = millis();
    static int servo_num = 0;
    static int S0 = 0; //Flank detector

    int time_difference_ms = 400; // Every 200 ms program will read the joystick val 

    bool change_axis_button = digitalRead(JOYSTICK_BUTTON_PIN);

    if ((millis() - StartTime) > time_difference_ms ) {

        float joys_x = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -JOYSTICK_NUMBER_OF_SPEEDS/2, JOYSTICK_NUMBER_OF_SPEEDS/2);

        if (abs(joys_x) < 1) joys_x = 0;

        float base_speed_increment = 0.5;

        servoinfo[servo_num].angle += (int) (base_speed_increment * joys_x);

        serial_write_angles();
        serial_write_xyz_from_anlges();
        Serial.write("-----------------------\n");

        StartTime = millis();

    }

    //--CHANGE MOTOR--//
    if (!change_axis_button && !S0) {

        servo_num = (servo_num + 1) % 3;
        S0 = 1; 

    } else if (change_axis_button) S0 = 0;

}

void joysitck_change_mode(int* change_joystick_mode) {

    static int S0 = 0;
    bool joystick_change_mode_button = digitalRead(JOYSTICK_CHANGE_AXIS_ANGLES);
    
    if (joystick_change_mode_button && !S0) {
    
      *change_joystick_mode = (*change_joystick_mode + 1) % 2;
      S0 = 1;
      
    }
    else if (!joystick_change_mode_button) S0 = 0;

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

void joystick_change_xy() {

    static unsigned long StartTime = millis();

    int time_difference_ms = 50; // Every 400 ms the program will read the joystick val 

    if ((millis() - StartTime) > time_difference_ms ) {

        float joys_x = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -JOYSTICK_NUMBER_OF_SPEEDS/2, JOYSTICK_NUMBER_OF_SPEEDS/2);
        float joys_y = map(analogRead(JOYSTICK_Y_PIN), 0, 1023, JOYSTICK_NUMBER_OF_SPEEDS/2, -JOYSTICK_NUMBER_OF_SPEEDS/2);

        if (abs(joys_x) < 1) joys_x = 0;
        if (abs(joys_y) < 1) joys_y = 0;

        float base_speed_increment = 1;

        deltainfo.x += joys_x * base_speed_increment;
        deltainfo.y += joys_y * base_speed_increment;

        serial_write_xyz();
        serial_write_angles();
        Serial.write("---------------------\n");

        StartTime = millis();
        
    }

}

/*********************************************************************
* Function: buttons_change_xyz();
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

void buttons_change_xyz() {

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
