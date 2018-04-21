#ifndef SERVO_MOVEMENT_H
	
	#include "servos.h"

	#define SERVO_MOVEMENT_H


	void move_selected_servos(bool move_servo1, bool move_servo2, bool move_servo3, bool move_servo4);
    void update_angle_from_dc(int servo_num);
    void update_dc_from_angle(int servo_num);
    void check_servo_change_direction(unsigned int num_servo, unsigned int new_duty_cycle);
    void init_ServoInfo(ServoInfo* servo_inf, int max_duty_cycle_, int min_duty_cycle_, int slack_compensation_val_, long m_, long n_);
    void servos_initial_positions(bool move_servo1, bool move_servo2, bool move_servo3, bool move_servo4);
    void init_servos(bool move_servo1, bool move_servo2, bool move_servo3, bool move_servo4);
    void set_servo_movement_with_dc(bool set_servo1, bool set_servo2, bool set_servo3, bool set_servo4);

    extern const long servo1_m;
    extern const long servo2_m;
    extern const long servo3_m;
    extern const long servo4_m;
    extern const long servo1_n;
    extern const long servo2_n;
    extern const long servo3_n;
    extern const long servo4_n;

#endif