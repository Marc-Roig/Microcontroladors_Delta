#include "Config.h"

//GLOBAL VARIABLES

//--SERIAL COMUNICATION--//

bool command_recieved = false;
int serial_mode = ASK_FOR_ANGLES;

void setup() {
  
  //--SERIAL COM--//
  Serial.begin(9600);
  
  // Serial.write("G20\n"); //Processing is waiting to arduino
  // Serial.write("G06\n");

  init_buffer(); 

  //--SERVOS--//
  init_servos(true, true, true, false);

  //--DELTA--//
  init_delta();

  //--CALIBRATION--//
  // calibration_start(true, true, true, false);

  //--JOYSTICK--//
  init_joystick();

}

void loop() {

  //--SERVOS--//
  move_servos(true, true, true, false); //Angle to duty cycle, only with three arm servos  

  //--SERIAL--//
  // serial_com_with_simulator();

  //--CALIBRATION--//
  // servo_calibration(true, true, true, false);

  //--JOYSTICK--//
  joystick_movement();
  
}





