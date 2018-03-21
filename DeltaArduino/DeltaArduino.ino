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
  Serial.begin(9600);
  
  Serial.write("G20\n"); //Processing is waiting to send
  Serial.write("G06\n");

  servos[0].attach(9);
  servos[1].attach(10);
  servos[2].attach(11);
  
  init_buffer(); 
}

void loop() {

  //--MOVEMENT--//
//  move_servo_to_limits(1, AngleStep, MinAngle, MaxAngle);
//  move_servo_to_limits(2, AngleStep2, MinAngle2, MaxAngle2);
//  move_servo_to_limits(3, AngleStep3, MinAngle3, MaxAngle3);

  servos[0].write(servos_angles[0]);
  servos[1].write(servos_angles[1]);
  servos[2].write(servos_angles[2]);

  //--SERIAL--//

  if(command_recieved) {
    parse_command(buffer.command[buffer.start]);
    if (inc_buffer_start_pointer()) {
      command_recieved = false; //If stack empty message has been read
      delay(SERIAL_DELAY_MS);
    }
  }
  
  else check_serial();
  // delay(30);
}





