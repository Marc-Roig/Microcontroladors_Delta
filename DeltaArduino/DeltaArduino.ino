#include <Servo.h>


//----SERVOS----//

#define AngleStep 12
#define retard 200
#define MinAngle 30
#define MaxAngle 140

#define AngleStep2 6 
#define retard2 200
#define MinAngle2 30
#define MaxAngle2 140
  
#define AngleStep3 6
#define retard3 200
#define MinAngle3 30
#define MaxAngle3 140

//----SERIAL----//

//SERIAL COM MODES
#define ASK_FOR_ANGLES  0
#define GIVE_ANGLES  1
#define MODE1  2

//GCODE CODES
#define ASKED_TO_LISTEN 0
#define PROCEED 1
#define TRANSMISSION_ENDED 2
#define MOVE_SERVOS 3
#define CHANGE_SPEED 4
#define MOVE_EF 5
#define END_OF_STREAM 6
#define SEND_MORE 7
#define ASK_TO_LISTEN 8
#define EMERGENCY_STOP 9

#define SEND_ANGLES 20
#define SEND_ANGLES_EFPOS 21

//SERIAL BUFFER

#define SERIAL_BUFFER_LEN 5
#define SERIAL_COMMAND_MAX_LEN 16


//--MOVEMENT--//
Servo servos[3];
int servos_angles[3];
int end_effector_pos[] = {0, 0, 100};
int end_effector_speed;

//--SERIAL COMUNICATION--//

typedef struct Buffer { //Circular buffer, FIFO
  char command[SERIAL_BUFFER_LEN][SERIAL_COMMAND_MAX_LEN];
  int command_len[SERIAL_BUFFER_LEN];
  int start, end_;
  bool full, empty;
}Buffer;

Buffer buffer;

bool command_recieved = false;
// bool command_send = false;

int serial_mode = ASK_FOR_ANGLES;

void setup() {
  Serial.begin(9600);
  
  Serial.write("G20\n"); //Processing is waiting to send
  Serial.write("G06\n");

  servos[0].attach(9);
  servos[1].attach(10);
  servos[2].attach(11);
  
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
    if (inc_buffer_start_pointer()) command_recieved = false; //If stack empty message has been read
  }
  
  else check_serial();
      

   delay(50);
}




