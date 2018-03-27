#include "Config.h"

void move_servos() {
  for (int i = 0; i < 3; i++) {
    //SCALE EVERY ANGLE (???)
  }
  for (int i = 0; i < 3; i++) {
    servos[i].write(servoinfo[i].angle);
  }
}

void init_ServoInfo(struct ServoInfo* servoinfo, int max_duty_cycle_, int min_duty_cycle_, int slack_compensation_val_) {

    servoinfo->angle = 90;
    
    servoinfo->max_duty_cycle = max_duty_cycle_;
    servoinfo->min_duty_cycle = min_duty_cycle_;

    servoinfo->mean_dc = (max_duty_cycle_ + min_duty_cycle_)/2; 
    servoinfo->duty_cycle = servoinfo->mean_dc - 500;

    servoinfo->last_direction = CLOCKWISE;
    servoinfo->slack_compensation_val = slack_compensation_val_;

}

//void move_servo_to_limits(int servo_num, int StepAngle, int min_ang, int max_ang) {
//  static int i = min_ang;
//  static int dir = 0;
//
//  if (servo_num > 3 || servo_num < 1) return; //Wrong servo num
//
//  if (dir) i -= StepAngle;
//  else i += StepAngle;
//
//  if (i > MaxAngle) i = max_ang;
//  if (i < MinAngle) i = min_ang;
//  
//  if (i == MaxAngle) dir = 1;
//  else if (i == MinAngle) dir = 0;
//
//  servos_angles[servo_num-1] = i; //Store the angle of the servo
//
//  servos[servo_num-1].write(i);
//}


