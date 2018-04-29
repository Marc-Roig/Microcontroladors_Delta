import controlP5.*;

ControlP5 cp5;

ButtonBar b;
Chart myservoChart1;
Chart myservoChart2;
Chart myservoChart3;

ControlGroup servo1ValBox;

Textlabel duty_cycle_val1;
Textlabel angle_val_s1;


//--SERVO DRAWING--//
PShape ServoBody;
PShape ServoHorn;

float scale_servo_draw = 0.4;
float scale_horn_draw = 0.2;

float angle1 = 0;
float angle2 = 0;
float angle3 = 0;

void setup() {
  size(1500, 800);
  cp5 = new ControlP5(this);

  change_mode_bar_init(cp5);

  chart_init();

  ServoBody = loadShape("Images/servoBody.svg");
  ServoBody.scale(scale_servo_draw);

  ServoHorn = loadShape("Images/servoHorn.svg");
  ServoHorn.scale(scale_horn_draw);

  cp5.setAutoDraw(false);

  create_servo_value_box();
}

void canvi_mode(int n) {
  println("bar clicked, item-value:", n);
}
float a= 0.0;
void draw() {
  background(220);

  draw_servos();
  angle_val_s1.setValue(nf(-angle1, 3, 1));

  myservoChart1.unshift("servo1Chart", -angle1);
  myservoChart2.unshift("servo2Chart", -angle2);
  myservoChart3.unshift("servo3Chart", -angle3);


  cp5.draw();

  draw_axis_charts();
}

void draw_servos() {

  //--FOLLOW MOUSE--//
  angle1 = degrees(atan2(mouseY-(height*0.25), (mouseX-(width * 0.15)+54)));
  angle2 = degrees(atan2(mouseY-(height*0.5), (mouseX-(width * 0.15)+54)));
  angle3 = degrees(atan2(mouseY-(height*0.75), (mouseX-(width * 0.15)+54)));

  //--DRAW SERVO 1--//
  pushMatrix();
  translate(width * 0.15, height*0.25);
  shape(ServoBody, - 320*scale_servo_draw, - 123*scale_servo_draw);
  translate(54, 0);
  rotate(radians(89.5 + angle1));
  shape(ServoHorn, - 189*scale_horn_draw, - 560*scale_horn_draw);
  popMatrix();

  //--DRAW SERVO 2--//
  pushMatrix();
  translate(width * 0.15, height*0.5);
  shape(ServoBody, - 320*scale_servo_draw, - 123*scale_servo_draw);
  translate(54, 0);
  rotate(radians(89.5 + angle2));
  shape(ServoHorn, - 189*scale_horn_draw, - 560*scale_horn_draw);
  popMatrix();

  //--DRAW SERVO 3--//
  pushMatrix();
  translate(width * 0.15, height*0.75);
  shape(ServoBody, - 320*scale_servo_draw, - 123*scale_servo_draw);
  translate(54, 0);
  rotate(radians(89.5 + angle3));
  shape(ServoHorn, - 189*scale_horn_draw, - 560*scale_horn_draw);
  popMatrix();
}

void change_mode_bar_init(ControlP5 cp5) {

  b = cp5.addButtonBar("canvi_mode")
    .setPosition(0, 0)
    .setSize(width, 40)
    .addItems(split("CALIBRATION SEQUENCE JOYSTICK", " "))
    .setFont(createFont("", 15))
    ;
  println(b.getItem("a"));
  //b.changeItem("a","text","first");
  //b.changeItem("b","text","second");
  //b.changeItem("c","text","third");
  //b.onMove(new CallbackListener(){
  //  public void controlEvent(CallbackEvent ev) {
  //    ButtonBar bar = (ButtonBar)ev.getController();
  //    println("hello ",bar.hover());
  //  }
  //});
}
void create_servo_value_box() {

  int font_size = 14;

  // create a group to store the servo values
  servo1ValBox = cp5.addGroup("servo1 Values", (int)(width * 0.75 - 100), (int)(height*0.25 - 65), 100);
  servo1ValBox.setBackgroundHeight(65);
  servo1ValBox.setBackgroundColor(color(134, 124, 134, 100));
  //servo1ValBox.hideBar();
  servo1ValBox.setFont(createFont("", 9));

  Textlabel duty_cycle_s1 = cp5.addTextlabel("DutyCycleLabelServo1", "DC:", 10, 6);
  duty_cycle_s1.setFont(createFont("", font_size));
  duty_cycle_s1.moveTo(servo1ValBox);

  duty_cycle_val1 = cp5.addTextlabel("DutyCycleValServo1", "512.3", 50, 6);
  duty_cycle_val1.setFont(createFont("", font_size));
  duty_cycle_val1.moveTo(servo1ValBox);

  Textlabel angle_s1 = cp5.addTextlabel("AngleLabelServo1", "θ:", 10, 34);
  angle_s1.setFont(createFont("", font_size));
  angle_s1.moveTo(servo1ValBox);

  angle_val_s1 = cp5.addTextlabel("AngleValServo1", "90", 50, 34);
  angle_val_s1.setFont(createFont("", font_size));
  angle_val_s1.moveTo(servo1ValBox);
  
  servo1ValBox.close();
}

void draw_axis_charts() {

  int s1_graph_ypos = (int)(height*0.25 - 75);
  int s1_graph_xpos = (int)(width * 0.3);

  float s1_graph_xsize = (width*0.45);
  int s1_graph_ysize = 150;

  int num_of_x_lines = 20;
  float perc_per_x_line = 1/float(num_of_x_lines);

  int num_of_y_lines = 6;
  float perc_per_y_line = 1/float(num_of_y_lines);
  int num_y_vals_to_show = 4;

  float total_time = 25; //in seconds 

  strokeWeight(1);
  stroke(100);
  textSize(9);
  textAlign(RIGHT);
  fill(0);

  for (int i = 0; i < num_of_y_lines + 1; i++) {
    line(s1_graph_xpos, s1_graph_ypos + s1_graph_ysize * (i*perc_per_y_line), s1_graph_xpos-10, s1_graph_ypos + s1_graph_ysize * (i*perc_per_y_line));  
    text((int)ceil(180*(1 - (i*perc_per_y_line)))-90, s1_graph_xpos - 15, s1_graph_ypos + s1_graph_ysize * (i*perc_per_y_line) + 4);
  }

  textAlign(CENTER);
  for (int i = 0; i < num_of_x_lines + 1; i++) {
    line(s1_graph_xpos + s1_graph_xsize * (i*perc_per_x_line), s1_graph_ypos, s1_graph_xpos + s1_graph_xsize * (i*perc_per_x_line), s1_graph_ypos - 10);
  }

  for (int i = 0; i < num_of_x_lines + 1; i+=num_of_x_lines/num_y_vals_to_show) {
    text(total_time*i*perc_per_x_line, s1_graph_xpos + s1_graph_xsize * (i*perc_per_x_line), s1_graph_ypos - 12);
  }

  strokeWeight(4);
  stroke(155);

  line(s1_graph_xpos - 2, s1_graph_ypos, s1_graph_xpos - 2, s1_graph_ypos + s1_graph_ysize); 
  line(s1_graph_xpos, s1_graph_ypos - 2, s1_graph_xpos + s1_graph_xsize, s1_graph_ypos - 2);
  line(s1_graph_xpos, s1_graph_ypos + 1 + s1_graph_ysize, s1_graph_xpos + s1_graph_xsize, s1_graph_ypos + s1_graph_ysize + 1); 
  line(s1_graph_xpos + s1_graph_xsize + 1, s1_graph_ypos, s1_graph_xpos  + s1_graph_xsize + 1, s1_graph_ypos + s1_graph_ysize); 

  strokeWeight(1);
  stroke(255, 255, 255, 20);

  for (float i = 1; i < num_of_y_lines; i++) {
    line(s1_graph_xpos, s1_graph_ypos + s1_graph_ysize * (i*perc_per_y_line), s1_graph_xpos + s1_graph_xsize, s1_graph_ypos + s1_graph_ysize * (i*perc_per_y_line));
  }
}

void chart_init() {


  int  s1_graph_ypos = (int)(height*0.25 - 75);
  int  s1_graph_xpos = (int)(width * 0.3);

  myservoChart1 = cp5.addChart("servo1")
    .setPosition(s1_graph_xpos, s1_graph_ypos)
    .setSize((int)(width*0.45), 150)
    .setRange(-90, 90)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    .setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setFont(createFont("", 13))
    ;

  myservoChart1.addDataSet("servo1Chart");
  myservoChart1.setData("servo1Chart", new float[300]);


  myservoChart2 = cp5.addChart("servo2")
    .setPosition(width * 0.3, height*0.5 - 75)
    .setSize(800, 150)
    .setRange(-90, 90)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    .setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setFont(createFont("", 13))
    ;

  myservoChart2.addDataSet("servo2Chart");
  myservoChart2.setData("servo2Chart", new float[300]);

  myservoChart3 = cp5.addChart("servo3")
    .setPosition(width * 0.3, height*0.75 - 75)
    .setSize(800, 150)
    .setRange(-90, 90)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    .setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setFont(createFont("", 13))
    ;

  myservoChart3.addDataSet("servo3Chart");
  myservoChart3.setData("servo3Chart", new float[300]);
}