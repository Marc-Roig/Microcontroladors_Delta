#include "Config.h"

//GLOBAL VARIABLES

//--MOVEMENT--//
Servo servos[3];
int servos_angles[3];

ServoInfo servoinfo[3];

int end_effector_pos[] = {0, 0, 100};
int end_effector_speed;

//--SERIAL COMUNICATION--//

Buffer buffer;

bool command_recieved = false;
int serial_mode = ASK_FOR_ANGLES;


void setup() {
  
  //--SERIAL COM--//
  Serial.begin(9600);
  
  // Serial.write("G20\n"); //Processing is waiting to send
  // Serial.write("G06\n");

  init_buffer(); 


  //--SERVOS--//
  servos[0].attach(9);
  servos[1].attach(10);
  servos[2].attach(11);


  //--CALIBRATION--//
  
  max_duty_cycles[0] = 2130;
  max_duty_cycles[1] = 2130;
  max_duty_cycles[2] = 2130;
  
  min_duty_cycles[0] = 300;
  min_duty_cycles[1] = 300;
  min_duty_cycles[2] = 300;
  
  change_mode_button_pin =  9;
  increase_dc_button_pin = 10;
  decrease_dc_button_pin = 11;
  change_step_change_pin = 12;

  //Every time the servo turns it has to compensate the slack of it 
  //Track of the previous direction is needed
  servo_direction[0] = CLOCKWISE;
  servo_direction[1] = CLOCKWISE;
  servo_direction[2] = CLOCKWISE;
  
  change_dir_compensation_val[0] = 20;
  change_dir_compensation_val[1] = 20;
  change_dir_compensation_val[2] = 0;
  
  init_ServoInfo(&servoinfo[0], 2130, 300, 20);
  init_ServoInfo(&servoinfo[1], 2130, 300, 20);
  init_ServoInfo(&servoinfo[2], 2130, 600, 0);

  pinMode(change_mode_button_pin, INPUT);
  pinMode(increase_dc_button_pin, INPUT);
  pinMode(decrease_dc_button_pin, INPUT);
  pinMode(change_step_change_pin, INPUT);

  calibration_initial_positions(true, true, true);
  
}

void loop() {

  //--SERVOS--//
  move_servos(servos_angles);  

  //--SERIAL--//
  // serial_com_with_simulator();

  //--CALIBRATION--//
  servo_calibration(true, false, false);
  
}






