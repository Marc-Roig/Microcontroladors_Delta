#ifndef KINEMATICS_H
	
	#define KINEMATICS_H


	
	int delta_calcForward(float theta1, float theta2, float theta3, float* x0, float* y0, float* z0);
	int delta_calcAngleYZ(float x0, float y0, float z0, float* theta);
	int delta_calcInverse(float x0, float y0, float z0, float* theta1, float* theta2, float* theta3);

	//SERIAL DEBUG
	void serial_write_xyz_from_angles();
	void serial_write_xyz();

	//UPDATE VALUES
	bool update_angles_from_xyz();
	bool update_xyz_from_angles();

	//DELTA INITIALIZATIONS
	void init_delta(int mode);

#endif