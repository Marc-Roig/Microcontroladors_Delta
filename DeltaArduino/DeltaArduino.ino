#include "Config.h"

//GLOBAL VARIABLES


int servoFrameMillis = 20;  // minimum time between servo updates

void setup() {

    //--SERIAL COM--//
    Serial_begin(9600);

    // Serial.write("G20\n"); //Processing is waiting to arduino
    Serial_write("G06\n"); 

    init_buffer(); 

    //--SERVO EASER--//
    for (int i = 0; i < 4; i++) {
        servoseased[i].init(i, servoFrameMillis);
    }
    
    //--SERVOS--//
    init_servos(true, true, true, true);

    //--DELTA--//
        //SEQUENCE_MODE 0
        //CALIBRATION_MODE 1
        //JOYSTICK_MODE 2
    init_delta(JOYSTICK_MODE);

    switch (deltainfo.mode) {

        case SEQUENCE_MODE:     //--JOYSTICK--//
                                init_joystick();

                                
                                //--SEQUENCE--//
                                init_sequence();

                                break;

        case CALIBRATION_MODE:  calibration_start(true, true, true, false);
                                break;

        case JOYSTICK_MODE:     init_joystick();
                                break;
                                
    }

}

void loop() {

    switch (deltainfo.mode){

        case SEQUENCE_MODE:     update_sequence_mode();
                                break;

        case CALIBRATION_MODE:  servo_calibration(true, true, true, false);
                                break;

        case JOYSTICK_MODE:     joystick_movement();
                                break;

    }

    //--SERVOS--//
    move_selected_servos(true, false, false, false); //Angle to duty cycle, only with three arm servos  

    //--SERIAL--//
    serial_com_with_simulator();


}

void update_sequence_mode() {

     //--SERVO EASER--//
    for (int i = 0; i < 4; i ++) {
        servoseased[i].update();
    }

    //--SEQUENCE--//
    if (!sequence.playing) {
        joystick_movement();
        sequence_update();
    }

}

// void start_servoeaser() {

//   int moves[4] = {2300, 200, 2300, 200};
//   int duration_betw_moves[4] = {2000, 2000, 2000, 2000} ; //In MS
//   //The lenght of each has to be the same

//   set_servo_movement_with_dc(true, true, true, false);
  
//   servoeaser.init(0, servoFrameMillis);
//   servoeaser.addMoves(moves, duration_betw_moves, 4); 
//   servoeaser.play();

// }




