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

#define SERIAL_DELAY_MS 100

//SERIAL COM MODES
#define ASK_FOR_ANGLES  0
#define GIVE_ANGLES  1

//GCODE CODES

#define MOVE_SERVOS 3
#define CHANGE_SPEED 4
#define MOVE_EF 5
#define END_OF_STREAM 6
#define EMERGENCY_STOP 9

#define SEND_ME_ANGLES 20
#define SEND_ANGLES_EFPOS 21

//SERIAL BUFFER

#define SERIAL_BUFFER_LEN 5
#define SERIAL_COMMAND_MAX_LEN 16


//---------------//

typedef struct Buffer { //Circular buffer, FIFO
  char command[SERIAL_BUFFER_LEN][SERIAL_COMMAND_MAX_LEN];
  int command_len[SERIAL_BUFFER_LEN];
  int start, end_;
  bool full, empty;
}Buffer;

