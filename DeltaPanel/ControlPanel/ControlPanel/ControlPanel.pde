import controlP5.*;
//import ChartAxis;

ControlP5 cp5;

ButtonBar b;
Chart myservoChart1;
Chart myservoChart2;
Chart myservoChart3;

ChartAxis myservoChartAxis1;
ChartAxis myservoChartAxis2;
ChartAxis myservoChartAxis3;

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

  cp5.getController("servo1").setBehavior(new TimedEvent());
  
  cp5.getController("servo1").getController().reset();
  // cp5.getController("servo1").setBehavior(new ControlBehavior() {
  //   public void update() {
  //     myservoChart1.unshift("servo1Chart", -angle1);
  //   }
  // }
  // );

  cp5.getController("servo2").setBehavior(new ControlBehavior() {
    public void update() {
      myservoChart2.unshift("servo2Chart", -angle2);
    }
  }
  );

  cp5.getController("servo3").setBehavior(new ControlBehavior() {
    public void update() {
      myservoChart3.unshift("servo3Chart", -angle3);
    }
  }
  );
    
}

// custom ControlBehavior
class TimedEvent extends ControlBehavior {
  long myTime;
  int interval = 200;

  public TimedEvent() { 
    reset();
  }
  void reset() { 
    myTime = millis() + interval;
  }

  public void update() {
    if (millis()>myTime) {
      myservoChart1.unshift("servo1Chart", -angle1); 
      reset();
    }
  }
}

void canvi_mode(int n) {
  println("bar clicked, item-value:", n);
}

float a= 0.0;
void draw() {
  background(220);

  draw_servos();
  angle_val_s1.setValue(nf(-angle1, 3, 1));

  
  myservoChart2.unshift("servo2Chart", -angle2);
  myservoChart3.unshift("servo3Chart", -angle3);

  cp5.draw();
  
  myservoChartAxis1.draw();
  myservoChartAxis2.draw();
  myservoChartAxis3.draw();

}

void draw_servos() {

  //--FOLLOW MOUSE--//
  angle1 = degrees(atan2(mouseY-(height*0.25), (mouseX-(width * 0.15)-54)));
  angle2 = degrees(atan2(mouseY-(height*0.5), (mouseX-(width * 0.15)-54)));
  angle3 = degrees(atan2(mouseY-(height*0.75), (mouseX-(width * 0.15)-54)));

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

void chart_init() {


  int  s1_graph_ypos = (int)(height*0.25 - 75);
  int  s1_graph_xpos = (int)(width * 0.3);

  myservoChart1 = cp5.addChart("servo1")
                  .setPosition(s1_graph_xpos, s1_graph_ypos)
                  .setSize((int)(width*0.5), 150)
                  .setRange(-90, 90)
                  .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
                  .setStrokeWeight(1.5)
                  .setColorCaptionLabel(color(40))
                  .setFont(createFont("", 13))
                  ;

  myservoChart1.addDataSet("servo1Chart");
  myservoChart1.setData("servo1Chart", new float[300]);
    
  myservoChartAxis1 = new ChartAxis(myservoChart1)
                      .enableXAxis()
                      .enableYAxis()
                      .enableHorGuides()
                      .enableContourn()
                      ;
  
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

  myservoChartAxis2 = new ChartAxis(myservoChart2)
                      .enableXAxis()
                      .enableYAxis()
                      .enableHorGuides()
                      .enableContourn()
                      ;

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

  myservoChartAxis3 = new ChartAxis(myservoChart3)
                      .enableXAxis()
                      .enableYAxis()
                      .enableHorGuides()
                      .enableContourn()
                      ;
}