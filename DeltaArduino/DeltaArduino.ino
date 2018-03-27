#include "Config.h"

//GLOBAL VARIABLES

//--MOVEMENT--//
Servo servos[3];
int servos_angles[3];
int end_effector_pos[] = {0, 0, 100};
int end_effector_speed;

//--SERIAL COMUNICATION--//

Buffer buffer;

bool command_recieved = false;
int serial_mode = ASK_FOR_ANGLES;


void setup() {
  
  //--SERIAL COM--//
  Serial.begin(9600);
  
  Serial.write("G20\n"); //Processing is waiting to send
  Serial.write("G06\n");

  init_buffer(); 


  //--SERVOS--//
  servos[0].attach(9);
  servos[1].attach(10);
  servos[2].attach(11);


  //--CALIBRATION--//
  pinMode(change_mode_button_pin, INPUT);
  pinMode(increase_dc_button_pin, INPUT);
  pinMode(decrease_dc_button_pin, INPUT);
  pinMode(change_step_change_pin, INPUT);

  calibration_initial_positions(true, true, true);
  
}

void loop() {

  //--SERVOS--//
  move_servos();  

  //--SERIAL--//
  serial_com_with_simulator();
  
}






