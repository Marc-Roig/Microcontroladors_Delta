void draw_triangle(float side_len) {
  fill(0);
  float x1 = 0;
  float y1 = -(side_len/ 2)/cos(radians(30));

  float x2 = side_len / 2;
  float y2 = side_len * tan(radians(30)) / 2;

  float x3 = -x2;
  float y3 = y2;
  triangle(x1, y1, x2, y2, x3, y3);
}

void draw_arm2(int i) {
  pushMatrix();

  float arm_angle = radians(-120)*i;
  rotate(arm_angle);

  float temp = e*tan(radians(30))*0.5;
  translate(0, temp);
  lower[i].x = modelX(0, 0, 0);
  lower[i].y = modelY(0, 0, 0);
  lower[i].z = modelZ(0, 0, 0);

  popMatrix();
}

void draw_arm(int i) {
  pushMatrix();

  float arm_angle = radians(-120)*i;
  rotate(arm_angle);

  float temp = f*tan(radians(30))*0.5;
  translate(0, temp);
  rotateX(radians(arms_angles[i]));
  rotateY(-PI/2);
  upper[i].x = modelX(0, rf, 0);
  upper[i].y = modelY(0, rf, 0);
  upper[i].z = modelZ(0, rf, 0);
  line(0, 0, 0, rf);

  popMatrix();
}

void draw_structure() {
  //--UPPER BODY--//
  pushMatrix();
  
  apply_transformations();
  rotateX(PI/2);

  for (int i = 0; i<3; i++) draw_arm(i);
  draw_triangle(f);

  popMatrix();
  
  //--------------------------------------//
  //--LOWER BODY--//
  pushMatrix();

  apply_transformations();
  translate(x_pos, z_pos, y_pos);
  rotateX(PI/2);

  for (int i = 0; i<3; i++) draw_arm2(i);
  draw_triangle(e);

  popMatrix();
  
  //--------------------------------------//
  //--DRAW RE's--//
  for (int arm_n = 0; arm_n < 3; arm_n++) {
    line(lower[arm_n].x, lower[arm_n].y, lower[arm_n].z, upper[arm_n].x, upper[arm_n].y, upper[arm_n].z);
  }
  
  //--------------------------------------//  
  //--DRAW GRID--//
  pushMatrix();
  apply_transformations();
  translate(0,z_pos+50);
  
  board.draw_grid();
  
  popMatrix();
}

void test_circular_movement() {
  x_pos = r * cos(angle);
  y_pos = r * sin(angle);
  
  angle += 0.1;
  if (angle >= 2*PI) angle = 0;
}