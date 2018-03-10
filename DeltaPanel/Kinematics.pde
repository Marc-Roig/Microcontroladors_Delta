float e = 110.0;     // end effector
float f = 220.0;     // base
float re = 200.0;
float rf = 70.0;

float sqrt3 = sqrt(3.0);
float pi = 3.141592653;    // PI
float sin120 = sqrt3/2.0;   
float cos120 = -0.5;        
float tan60 = sqrt3;
float sin30 = 0.5;
float tan30 = 1/sqrt3;

float delta_calcAngleYZ(float x0, float y0, float z0, float previous_angle) {
  float y1 = -0.5 * 0.57735 * f; // f/2 * tg 30
  y0 -= 0.5 * 0.57735 * e;    // shift center to edge
  // z = a + b*y
  float a = (x0*x0 + y0*y0 + z0*z0 +rf*rf - re*re - y1*y1)/(2*z0);
  float b = (y1-y0)/z0;
  // discriminant
  float d = -(a+b*y1)*(a+b*y1)+rf*(b*b*rf+rf); 
  if (d < 0) return -400; // non-existing point
  float yj = (y1 - a*b - sqrt(d))/(b*b + 1); // choosing outer point
  float zj = a + b*yj;
  float theta = 180.0*atan2(-zj,(y1 - yj))/pi;
  return theta;
}

float angle_limit_1 = -90;
float angle_limit_2 = 90;

boolean delta_calcInverse(float x0, float y0, float z0, float[] angles) {
  float angle0 = delta_calcAngleYZ(x0, y0, z0, angles[0]);
  float angle1 = delta_calcAngleYZ(x0*cos120 + y0*sin120, y0*cos120-x0*sin120, z0, angles[1]);  // rotate coords to +120 deg
  float angle2 = delta_calcAngleYZ(x0*cos120 - y0*sin120, y0*cos120+x0*sin120, z0, angles[2]);  // rotate coords to -120 deg
  
  //println(angle0, angle1, angle2);
  if (angle0 != -400 && angle0 > angle_limit_1 && angle0 < angle_limit_2) {
    if (angle1 != -400 && angle1 > angle_limit_1 && angle1 < angle_limit_2) {
      if (angle2 != -400 && angle2 > angle_limit_1 && angle2 < angle_limit_2) {
        angles[0] = angle0;
        angles[1] = angle1;
        angles[2] = angle2;
    
    return true;
      }  
    }
  }
  return false;
}