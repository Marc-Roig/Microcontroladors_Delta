import controlP5.*;
//import ChartAxis;

ControlP5 cp5;

ButtonBar b;

ServoInfo servoinfo[] = new ServoInfo[3];

float angle1 = 0;
float angle2 = 0;
float angle3 = 0;

public class ServoInfo {

  float angle;

  public ServoInfo(int angle_) {

    angle = angle_;

  }

}

//--SETUP--//

void setup() {

  size(1500, 800);

  cp5 = new ControlP5(this);

  servoinfo[0] = new ServoInfo(0);
  servoinfo[1] = new ServoInfo(0);
  servoinfo[2] = new ServoInfo(0);

  change_mode_bar_init(cp5);
  init_ServoVisualTab();
  

  cp5.setAutoDraw(false);

    
}


void canvi_mode(int n) {
  println("bar clicked, item-value:", n);
}

void draw() {

  background(220);

  draw_servos();

  servoinfo[0].angle = angle1;
  servoinfo[1].angle = angle2;
  servoinfo[2].angle = angle3;

  cp5.draw();
  
  myservoChartAxis1.draw();
  myservoChartAxis2.draw();
  myservoChartAxis3.draw();

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