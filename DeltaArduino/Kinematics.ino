#include "Config.h"

DeltaInfo deltainfo;

 // robot geometry
 // (look at pics above for explanation)
 const float e = 150.0;     // end effector
 const float f = 207.85;     // base
 const float re = 200.0;
 const float rf = 100.0;
 
 // trigonometric constants
 const float sqrt3 = 1.7320508;
 const float pi = 3.141592653;    // PI
 const float sin120 = sqrt3/2.0;   
 const float cos120 = -0.5;        
 const float tan60 = sqrt3;
 const float sin30 = 0.5;
 const float tan30 = 1/sqrt3;
 
 // forward kinematics: (theta1, theta2, theta3) -> (x0, y0, z0)
 // returned status: 0=OK, -1=non-existing position
int delta_calcForward(float theta1, float theta2, float theta3, float* x0, float* y0, float* z0) {

     float t = (f-e)*tan30/2;
     float dtr = pi/(float)180.0;

     theta1 *= dtr;
     theta2 *= dtr;
     theta3 *= dtr;

     float y1 = -(t + rf*cos(theta1));
     float z1 = -rf*sin(theta1);

     float y2 = (t + rf*cos(theta2))*sin30;
     float x2 = y2*tan60;
     float z2 = -rf*sin(theta2);

     float y3 = (t + rf*cos(theta3))*sin30;
     float x3 = -y3*tan60;
     float z3 = -rf*sin(theta3);

     float dnm = (y2-y1)*x3-(y3-y1)*x2;

     float w1 = y1*y1 + z1*z1;
     float w2 = x2*x2 + y2*y2 + z2*z2;
     float w3 = x3*x3 + y3*y3 + z3*z3;

     // x = (a1*z + b1)/dnm
     float a1 = (z2-z1)*(y3-y1)-(z3-z1)*(y2-y1);
     float b1 = -((w2-w1)*(y3-y1)-(w3-w1)*(y2-y1))/2.0;

     // y = (a2*z + b2)/dnm;
     float a2 = -(z2-z1)*x3+(z3-z1)*x2;
     float b2 = ((w2-w1)*x3 - (w3-w1)*x2)/2.0;

     // a*z^2 + b*z + c = 0
     float a = a1*a1 + a2*a2 + dnm*dnm;
     float b = 2*(a1*b1 + a2*(b2-y1*dnm) - z1*dnm*dnm);
     float c = (b2-y1*dnm)*(b2-y1*dnm) + b1*b1 + dnm*dnm*(z1*z1 - re*re);

     // discriminant
     float d = b*b - (float)4.0*a*c;
     if (d < 0) return -1; // non-existing point

     *z0 = -(float)0.5*(b+sqrt(d))/a;
     *x0 = (a1*(*z0) + b1)/dnm;
     *y0 = (a2*(*z0) + b2)/dnm;
     return 0;

}
 
 // inverse kinematics
 // helper functions, calculates angle theta1 (for YZ-pane)
int delta_calcAngleYZ(float x0, float y0, float z0, float* theta) {

     float y1 = -0.5 * 0.57735 * f; // f/2 * tg 30
     y0 -= 0.5 * 0.57735 * e;    // shift center to edge

     // z = a + b*y
     float a = (x0*x0 + y0*y0 + z0*z0 +rf*rf - re*re - y1*y1)/(2*z0);
     float b = (y1-y0)/z0;

     // discriminant
     float d = -(a+b*y1)*(a+b*y1)+rf*(b*b*rf+rf); 

     if (d < 0) return -1; // non-existing point

     float yj = (y1 - a*b - sqrt(d))/(b*b + 1); // choosing outer point
     float zj = a + b*yj;

     *theta = 180.0*atan(-zj/(y1 - yj))/pi + ((yj>y1)?180.0:0.0);

     return 0;

}
 
 // inverse kinematics: (x0, y0, z0) -> (theta1, theta2, theta3)
 // returned status: 0=OK, -1=non-existing position
int delta_calcInverse(float x0, float y0, float z0, float* theta1, float* theta2, float* theta3) {

     *theta1 = 0;
     *theta2 = 0;
     *theta3 = 0;

     int status = delta_calcAngleYZ(x0, y0, z0, theta1);
     if (status == 0) status = delta_calcAngleYZ(x0*cos120 + y0*sin120, y0*cos120-x0*sin120, z0, theta2);  // rotate coords to +120 deg
     if (status == 0) status = delta_calcAngleYZ(x0*cos120 - y0*sin120, y0*cos120+x0*sin120, z0, theta3);  // rotate coords to -120 deg

     return status;

}

void serial_write_xyz_from_angles() {

     float x = 0;
     float y = 0;
     float z = 0;

     int is_valid = delta_calcForward((float)servoinfo[0].angle - 90, (float)servoinfo[1].angle - 90, (float)servoinfo[2].angle - 90, &x, &y, &z);

     if (is_valid != -1) {

          Serial_write("X - ");
          Serial_print(x);
          Serial_write("   Y - ");
          Serial_print(y);
          Serial_write("   Z - ");
          Serial_println(z);

     } 
     // else Serial_write("INVALID POSITION\n");

}

void serial_write_xyz() {

     Serial_write("X - ");
     Serial_print(deltainfo.x);
     Serial_write("   Y - ");
     Serial_print(deltainfo.y);
     Serial_write("   Z - ");
     Serial_println(deltainfo.z);

}

bool update_angles_from_xyz() {

     float theta1 = 0;
     float theta2 = 0;
     float theta3 = 0;

     int is_valid = delta_calcInverse(deltainfo.x, deltainfo.y, deltainfo.z, &theta1, &theta2, &theta3);

     if (is_valid == 0) {

          servoinfo[0].angle = theta1 + 90.0;
          servoinfo[1].angle = theta2 + 90.0;
          servoinfo[2].angle = theta3 + 90.0;

          return true;
          
     }
     else Serial_write("Invalid pos\n");

     return false; //Invalid position

}

bool update_xyz_from_angles() {

     float x = 0;
     float y = 0;
     float z = 0;

     int is_valid = delta_calcForward((float)servoinfo[0].angle - 90, (float)servoinfo[1].angle - 90, (float)servoinfo[2].angle - 90, &x, &y, &z);

     if (is_valid != -1) {

          deltainfo.x = x;
          deltainfo.y = y;
          deltainfo.z = z;

          return true;

     } //else Serial_write("INVALID POSITION\n");

     return false;

}

void init_delta(int mode) {

     deltainfo.x = 0;
     deltainfo.y = 0;
     deltainfo.z = -162.5;

     deltainfo.mode = mode;
     set_servo_movement_with_dc(true, true, true, true);

}
