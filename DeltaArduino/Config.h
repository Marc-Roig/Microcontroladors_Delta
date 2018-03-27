#ifndef CONFIG_H
  
  #include <Servo.h>

  #define CONFIG_H
  
  //----CALIBRATION----//
  //CHANGE DC MODE
  #define CHANGE_WITH_BUTTONS 0
  #define CHANGE_WITH_POTENTIOMETER 1
  
  //SERVO MOVEMENT
  #define CLOCKWISE 0
  #define COUNTERCLOCKWISE 1
  
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

  typedef struct ServoInfo {

    int angles[3];

    int duty_cycle;

    int max_duty_cycle;
    int min_duty_cycle;

    int mean_dc;

    int last_direction;
    int slack_compensation_val;

  }ServoInfo;
  
  int max_duty_cycles[3];
  int min_duty_cycles[3];
  
  int change_mode_button_pin;
  int increase_dc_button_pin;
  int decrease_dc_button_pin;
  int change_step_change_pin;
  
  int servo_direction[3];
  int change_dir_compensation_val[3];

#endif
