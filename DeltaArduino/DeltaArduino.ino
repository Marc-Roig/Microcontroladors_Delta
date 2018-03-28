#include "Config.h"

//GLOBAL VARIABLES

int end_effector_pos[] = {0, 0, 100};
int end_effector_speed;

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
  init_servos();

  //--CALIBRATION--//
  calibration_start(true, true, true);

}

void loop() {

  //--SERVOS--//
  move_servos(true, true, true);  

  //--SERIAL--//
  // serial_com_with_simulator();

  //--CALIBRATION--//
  servo_calibration(false, false, false);
  
}






