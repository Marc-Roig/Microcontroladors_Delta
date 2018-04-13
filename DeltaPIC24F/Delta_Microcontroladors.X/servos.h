#ifndef SERVOS_H

	#define SERVOS_H

    // typedef struct ServoInfo {

    //     float angle;

    //     int duty_cycle;
    //     int dc_offset;

    //     int max_duty_cycle;
    //     int min_duty_cycle;

    //     int mean_dc;

    //     int last_direction;
    //     int slack_compensation_val;

    //     //--ANGLE TO DUTY_CYCLE
    //     float m;
    //     float n;

    //     char* move_servo_from; //Change position with angles/microseconds/xyz

    // }ServoInfo;

	void disengage_servos();

    void engage_servos();

    void servo1_write(int angle);
    void servo2_write(int angle);
    void servo3_write(int angle);

    void servo1_write_duty_cycle(int duty_cycle);
    void servo2_write_duty_cycle(int duty_cycle);
    void servo3_write_duty_cycle(int duty_cycle);

    int scale_angle(int angle, int servo_num);

#endif //SERVOS_H