#include "Config.h"

//GLOBAL VARIABLES

//--SERIAL COMUNICATION--//

bool command_recieved = false;
int serial_mode = ASK_FOR_ANGLES;

ServoEaser servoseased[4];

int servoFrameMillis = 20;  // minimum time between servo updates

void setup() {
  
  //--SERIAL COM--//
  Serial.begin(9600);
  
  // Serial.write("G20\n"); //Processing is waiting to arduino
  // Serial.write("G06\n");

  init_buffer(); 

  //--SERVOS--//
  init_servos(true, false, false, false);

  //--DELTA--//
  init_delta();

  //--CALIBRATION--//
  calibration_start(true, true, true, false);

  //--JOYSTICK--//
  // init_joystick();
  
  //--SERVO EASER--//
  for (int i = 0; i < 4; i++) {
    servoseased[i].init(servos[i], i, servoFrameMillis);
  }

  // start_servoeaser();

  //--SEQUENCE--//
  init_sequence();
  

}

void loop() {

    //--SERVOS--//
    move_servos(true, false, false, false); //Angle to duty cycle, only with three arm servos  

    //--SERIAL--//
    // serial_com_with_simulator();

    //--CALIBRATION--//
    // servo_calibration(true, true, true, false);

    //--JOYSTICK--//
    // joystick_movement();

    //--SERVO EASER--//
    // servoeaser.update();

    for (int i = 0; i < 4; i ++) {
        servoseased[i].update();
    }

    //--SEQUENCE--//
    if (!sequence.playing) {
        servo_calibration(true, true, true, false);
        sequence_update();
    }


}

void start_servoeaser() {

  int moves[4] = {2300, 200, 2300, 200};
  int duration_betw_moves[4] = {2000, 2000, 2000, 2000} ; //In MS
  //The lenght of each has to be the same

  set_servo_movement_with_dc(true, true, true, false);
  
  servoeaser.init(servos[0], 0, servoFrameMillis);
  servoeaser.addMoves(moves, duration_betw_moves, 4); 
  servoeaser.play();

}




