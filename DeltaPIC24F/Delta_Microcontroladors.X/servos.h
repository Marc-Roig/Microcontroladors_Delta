#ifndef SERVOS_H

	#define SERVOS_H

	int scale_angle(int angle, int servo_num);
    void servo1_write(int angle);
    void servo2_write(int angle);
    void servo3_write(int angle);
    extern void servo1_writeMicroseconds(int microsec);
    extern void servo2_writeMicroseconds(int microsec);
    extern void servo3_writeMicroseconds(int microsec);
    void servo_writeMicroseconds(int microsec, int servo_num);
    void disengage_servos();
    void servo_attach(int pin, int servo_num);
    void attach_servos();

#endif //SERVOS_H