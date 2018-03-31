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
  calibration_start(true, true, true, false);

}

void loop() {

  //--SERVOS--//
  move_servos_from_angle(false, false, false); //Angle to duty cycle, only with three arm servos  

  //--SERIAL--//
  // serial_com_with_simulator();

  //--CALIBRATION--//
  servo_calibration(true, true, true, false);
  
}

void init_delta() {

  // deltainfo.x = 

}






