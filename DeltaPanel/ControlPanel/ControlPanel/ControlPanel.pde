import controlP5.*;
import processing.serial.*;

//--COMUNICAITON--//

Serial myPort;
Buffer buffer;

ControlP5 cp5;

ButtonBar b;
ControlTimer c;
Textlabel timerLabel;


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

  


  servoinfo[0] = new ServoInfo(0);
  servoinfo[1] = new ServoInfo(0);
  servoinfo[2] = new ServoInfo(0);

  choosePort();
  buffer = new Buffer();

  //CP5

  cp5 = new ControlP5(this);

  initBottomStatusBar();

  change_mode_bar_init(cp5);
  init_ServoVisualTab();
  
  init_console();
  ConsoleSetMax(20);
  // write_console("aaaa\n");

  cp5.setAutoDraw(false);

    
}

void initBottomStatusBar() {

  //--TIMER--//

  c = new ControlTimer();
  timerLabel = new Textlabel(cp5,"--",100,100);
  c.setSpeedOfTime(1);
  timerLabel.setPosition(width * 0.93, height * 0.96);
  timerLabel.setFont(createFont("", 16));

  PImage ConsoleBright = loadImage("Images/console_brighter.png");
  ConsoleBright.resize(30,30);
  PImage ConsoleDark =  loadImage("Images/console_bright.png");
  ConsoleDark.resize(30, 30);
  
  cp5.addIcon("ConsoleIcon", 10)
     .setPosition(width * 0.89, height * 0.96 - 2)
     .setValue(0)
     .setSize(70,50)
     .setRoundedCorners(20)
     .setImages(ConsoleDark, ConsoleBright, ConsoleBright)
     .setSwitch(true)
     .setColorBackground(color(255,100))
     .hideBackground()
     ;  


}

void ConsoleIcon(boolean theValue) {

  if (ConsoleTextArea == null) return;
  ConsoleTextArea.hide();
  if (theValue) ConsoleTextArea.show();
  else ConsoleTextArea.hide();

}

void drawBottomStatusBar() {

  fill(2, 31, 75);
  // noStroke();
  stroke(4, 55, 105);
  rect(-10, height * 0.95, width+10, height+10); //draw outside canvas to hide stroke
  stroke(7, 120, 190, 70);
  strokeWeight(2);
  line(-10, height *0.95 +2, width+10, height *0.95 +2);

  stroke(220, 220, 220, 50);
  line(width * 0.92, height * 0.95 + 7, width * 0.92, height - 7);
  line(width * 0.88, height * 0.95 + 7, width * 0.88, height - 7);

}

void canvi_mode(int n) {
  println("bar clicked, item-value:", n);
}

int a = 0;

void draw() {

  background(220);

  draw_servos();

  servoinfo[0].angle = angle2;
  servoinfo[1].angle = angle2;
  servoinfo[2].angle = angle3;

  drawBottomStatusBar();

  write_console(String.valueOf(a++)+"\n");
  cp5.draw();

  timerLabel.setValue(c.toString());
  timerLabel.draw(this);
  
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