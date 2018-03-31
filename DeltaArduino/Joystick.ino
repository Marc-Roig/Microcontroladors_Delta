#include "Config.h"

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

    static int StartTime = millis();

    int time_difference_ms = 200; // Every 200 ms program will read the joystick val 
    // int joystick_number_of_speeds = 10;

    if ((millis() - StartTime) > time_difference_ms ) {

        float joys_x = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -JOYSTICK_NUMBER_OF_SPEEDS/2, JOYSTICK_NUMBER_OF_SPEEDS/2);
        float joys_y = map(analogRead(JOYSTICK_Y_PIN), 0, 1023, -JOYSTICK_NUMBER_OF_SPEEDS/2, JOYSTICK_NUMBER_OF_SPEEDS/2);

        float base_speed_increment = 0.1;

        deltainfo.x += joys_x * base_speed_increment; 
        deltainfo.y += joys_y * base_speed_increment; 

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
    bool change_axis_button = digitalRead(BUTTONS_CHANGE_AXIS_XYZ_PIN);
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
    deltainfo.x = new_x;
    deltainfo.y = new_y;

}