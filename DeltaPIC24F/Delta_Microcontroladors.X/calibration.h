#ifndef CALIBRATION_H

	#define CALIBRATION_H
 
	void servo_calibration(bool move_servo1, bool move_servo2, bool move_servo3, bool move_servo4);
	void debug_calibration(int wait_time);
	void calibration_start();
	void calibration_change_dc_mode(int* change_dc_mode);
	void calibration_change_dc_potentiometer(int servo_num);
	void calibration_change_dc_buttons(int servo_num);
	 
#endif /* CALIBRATION_H */