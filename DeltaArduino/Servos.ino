#include "Config.h"

void move_servos(bool move_servo1, bool move_servo2, bool move_servo3) {

  bool move_servos[] = {move_servo1, move_servo2, move_servo3};

  int new_duty_cycle;

  for (int i = 0; i < 3; i++) {

    new_duty_cycle = servoinfo[i].angle * servoinfo[i].m + servoinfo[i].n;

    //SLACK COMPENSATION
    if (new_duty_cycle > servoinfo[i].duty_cycle + 20) {

      if (servoinfo[i].last_direction == COUNTERCLOCKWISE) {

        servoinfo[i].last_direction = CLOCKWISE;
        new_duty_cycle += servoinfo[i].slack_compensation_val;

      }

    }
    else if (new_duty_cycle < servoinfo[i].duty_cycle - 20) {

      if (servoinfo[i].last_direction == CLOCKWISE) {

        servoinfo[i].last_direction = COUNTERCLOCKWISE;
        new_duty_cycle -= servoinfo[i].slack_compensation_val;

      }

    }


  }

  for (int i = 0; i < 3; i++) {
    if (move_servos[i]) {
      servos[i].writeMicroseconds(servoinfo[i].duty_cycle);
      servoinfo[i].duty_cycle = new_duty_cycle;
    }
  }

}

void init_ServoInfo(struct ServoInfo* servo_inf, int max_duty_cycle_, int min_duty_cycle_, int slack_compensation_val_, float m_, float n_) {

    servo_inf->angle = 90;
    
    servo_inf->max_duty_cycle = max_duty_cycle_;
    servo_inf->min_duty_cycle = min_duty_cycle_;

    servo_inf->mean_dc = (max_duty_cycle_ + min_duty_cycle_)/2; 
    servo_inf->duty_cycle = servo_inf->mean_dc - 500;

    servo_inf->last_direction = CLOCKWISE;
    servo_inf->slack_compensation_val = slack_compensation_val_;

    servo_inf->m = m_;
    servo_inf->n = n_;
}

void init_servos() {
  
  servos[0].attach(SERVO1_PIN);
  servos[1].attach(SERVO2_PIN);
  servos[2].attach(SERVO3_PIN);
  servos[3].attach(SERVO3_PIN);

  init_ServoInfo(&servoinfo[0], MAX_DC_SERVO1, MIN_DC_SERVO1, SERVO1_COMPENSATION_VAL, SERVO1_M_ANGLE_TO_DC, SERVO1_N_ANGLE_TO_DC);
  init_ServoInfo(&servoinfo[1], MAX_DC_SERVO2, MIN_DC_SERVO2, SERVO2_COMPENSATION_VAL, SERVO2_M_ANGLE_TO_DC, SERVO2_N_ANGLE_TO_DC);
  init_ServoInfo(&servoinfo[2], MAX_DC_SERVO3, MIN_DC_SERVO3, SERVO3_COMPENSATION_VAL, SERVO3_M_ANGLE_TO_DC, SERVO3_N_ANGLE_TO_DC);
  init_ServoInfo(&servoinfo[3], MAX_DC_SERVO4, MIN_DC_SERVO4, SERVO4_COMPENSATION_VAL, SERVO3_M_ANGLE_TO_DC, SERVO3_N_ANGLE_TO_DC);

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


