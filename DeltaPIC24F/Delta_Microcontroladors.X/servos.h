#ifndef SERVOS_H
	
	#define SERVOS_H

	void disengage_servos();

    void engage_servos();

    void servo1_write(int angle);
    void servo2_write(int angle);
    void servo3_write(int angle);

    void servo1_write_duty_cycle(int duty_cycle);
    void servo2_write_duty_cycle(int duty_cycle);
    void servo3_write_duty_cycle(int duty_cycle);

    int scale_angle(int angle);

#endif //SERVOS_H