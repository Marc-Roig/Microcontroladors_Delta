void get_limits() {
  status = true;
  int count = 0;
  z_pos = z_start;
  while(z_pos < z_end) {
    
    x_pos = r * cos(0);
    y_pos = r * sin(0);
    status = delta_calcInverse(x_pos, -y_pos, z_pos, arms_angles);

    if (status) r += 0.1;
    else {
      angles_limits[count][0] = z_pos;
      angles_limits[count][1] = r;
      //println(z_pos);
      //println("0,"+Integer.toString(int(r%1*1000)));
      println(int(r));

      count++;
      z_pos += 0.5;
      r=1;
    }
  }
 
}