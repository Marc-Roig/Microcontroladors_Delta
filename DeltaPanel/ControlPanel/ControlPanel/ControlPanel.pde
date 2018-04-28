import controlP5.*;

ControlP5 cp5;


//--SERVO DRAWING--//
PShape ServoBody;
PShape ServoHorn;

float scale_servo_draw = 0.4;
float scale_horn_draw = 0.2;

void setup() {
  size(1500, 800);
  cp5 = new ControlP5(this);
  
  change_mode_bar_init(cp5);
  
  ServoBody = loadShape("Images/servoBody.svg");
  ServoBody.scale(scale_servo_draw);
  
  ServoHorn = loadShape("Images/servoHorn.svg");
  ServoHorn.scale(scale_horn_draw);
  
}


void canvi_mode(int n) {
  println("bar clicked, item-value:", n);
}

void draw() {
  background(220);
  
  pushMatrix();
  shape(ServoBody, width * 0.15 - 320*scale_servo_draw, height*0.25 - 123*scale_servo_draw);
  shape(ServoBody, width * 0.15 - 320*scale_servo_draw, height*0.5 - 123*scale_servo_draw);
  shape(ServoBody, width * 0.15 - 320*scale_servo_draw, height*0.75 - 123*scale_servo_draw);
  
  shape(ServoHorn, width * 0.15 - 189*scale_horn_draw, height*0.25 - 560*scale_horn_draw);
  //strokeWeight(10);
  //point(width/2, height/2);
  popMatrix();

}

void change_mode_bar_init(ControlP5 cp5) {

  ButtonBar b = cp5.addButtonBar("canvi_mode")
     .setPosition(0, 0)
     .setSize(width, 40)
     .addItems(split("CALIBRATION SEQUENCE JOYSTICK"," "))
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