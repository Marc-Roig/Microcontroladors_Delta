#include "Config.h"

//GLOBAL VARIABLES

//--SERIAL COMUNICATION--//

bool command_recieved = false;
int serial_mode = ASK_FOR_ANGLES;

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
  // calibration_start(true, true, true, false);

  //--JOYSTICK--//
  // init_joystick();

  //--SERVO EASER--//
  start_servoeaser();

}

void loop() {

  //--SERVOS--//
  // move_servos(true, false, false, false); //Angle to duty cycle, only with three arm servos  

  //--SERIAL--//
  // serial_com_with_simulator();

  //--CALIBRATION--//
  // servo_calibration(true, true, true, false);

  //--JOYSTICK--//
  // joystick_movement();

  //--SERVO EASER--//
  servoeaser.update();
  
}

void start_servoeaser() {

  int moves[4] = {1900, 1200, 1900, 1200};
  int duration_betw_moves[4] = {1000, 1000, 1000, 1000} ; //In MS
  //The lenght of each has to be the same

  set_servo_movement_with_dc(true, true, true, false);
  
  servoeaser.init(servos[0], 0, servoFrameMillis);
  servoeaser.addMoves(moves, duration_betw_moves, 4); 
  servoeaser.play();

}




