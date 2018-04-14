#ifndef SERVO_MOVEMENT_H
	
	#include "servos.h"

	#define SERVO_MOVEMENT_H

    typedef struct ServoInfo {

        float angle;

        int duty_cycle;
        int dc_offset;

        int max_duty_cycle;
        int min_duty_cycle;

        int mean_dc;

        int last_direction;
        int slack_compensation_val;

        //--ANGLE TO DUTY_CYCLE
        float m;
        float n;

        char* move_servo_from; //Change position with angles/microseconds/xyz

    }ServoInfo;

	void move_servos(bool move_servo1, bool move_servo2, bool move_servo3, bool move_servo4);
	void update_angle_from_dc(int servo_num);

	void update_dc_from_angle(int servo_num);

	void check_servo_change_direction(int num_servo, int new_duty_cycle);
	void init_ServoInfo(struct ServoInfo* servo_inf, int max_duty_cycle_, int min_duty_cycle_, int slack_compensation_val_, float m_, float n_);
	void servos_initial_positions(bool move_servo1, bool move_servo2, bool move_servo3, bool move_servo4);
	void init_servos(bool move_servo1, bool move_servo2, bool move_servo3, bool move_servo4);
	void set_servo_movement_with_dc(bool set_servo1, bool set_servo2, bool set_servo3, bool set_servo4);


#endif