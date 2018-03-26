void move_servos(int angles[3]) {
  for (int i = 0; i < 3; i++) {
    //SCALE EVERY ANGLE (???)
  }
  for (int i = 0; i < 3; i++) {
    servos[i].write(angles[i]);
  }
}

void move_servo_to_limits(int servo_num, int StepAngle, int min_ang, int max_ang) {
  static int i = min_ang;
  static int dir = 0;

  if (servo_num > 3 || servo_num < 1) return; //Wrong servo num

  if (dir) i -= StepAngle;
  else i += StepAngle;

  if (i > MaxAngle) i = max_ang;
  if (i < MinAngle) i = min_ang;
  
  if (i == MaxAngle) dir = 1;
  else if (i == MinAngle) dir = 0;

  servos_angles[servo_num-1] = i; //Store the angle of the servo

  servos[servo_num-1].write(i);
}

// void update_all_servos(){
//   servos[0].write(servos_angles[0]);
//   servos[1].write(servos_angles[1]);
//   servos[2].write(servos_angles[2]);
// }

