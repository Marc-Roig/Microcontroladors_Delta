  
Chart myservoChart1;
Chart myservoChart2;
Chart myservoChart3;

ChartAxis myservoChartAxis1;
ChartAxis myservoChartAxis2;
ChartAxis myservoChartAxis3;

ChartBox myservoBox1;
ChartBox myservoBox2;
ChartBox myservoBox3;

ControlGroup servo1ValBox;

//--SERVO DRAWING--//
PShape ServoBody;
PShape ServoHorn;


float scale_servo_draw = 0.4;
float scale_horn_draw = 0.2;

void init_ServoVisualTab() {

    chart_init();

    cp5.addToggle("StopServo1Chart")
     .setPosition(width * 0.85 , myservoChart1.getPosition()[1])
     .setSize(50,20)
     .setValue(true)
     .setMode(ControlP5.SWITCH)
     .setColorLabel(0)
     .setLabel("STOP GRAPH 1")
     .setFont(createFont("", 10))
     ;

    cp5.addToggle("StopServo2Chart")
     .setPosition(width * 0.85 , myservoChart2.getPosition()[1])
     .setSize(50,20)
     .setValue(true)
     .setMode(ControlP5.SWITCH)
     .setColorLabel(0)
     .setLabel("STOP GRAPH 2")
     .setFont(createFont("", 10))
     ;

    cp5.addToggle("StopServo3Chart")
     .setPosition(width * 0.85 , myservoChart3.getPosition()[1])
     .setSize(50,20)
     .setValue(true)
     .setMode(ControlP5.SWITCH)
     .setColorLabel(0)
     .setLabel("STOP GRAPH 3")
     .setFont(createFont("", 10))
     ;

    ServoBody = loadShape("Images/servoBody.svg");
    ServoBody.scale(scale_servo_draw);

    ServoHorn = loadShape("Images/servoHorn.svg");
    ServoHorn.scale(scale_horn_draw);

    cp5.getController("servo1").setBehavior(new TimedEvent("servo1Chart", "servo1", myservoChartAxis1, servoinfo[0]));
    cp5.getController("servo2").setBehavior(new TimedEvent("servo2Chart", "servo2", myservoChartAxis2, servoinfo[1]));
    cp5.getController("servo3").setBehavior(new TimedEvent("servo3Chart", "servo3", myservoChartAxis3, servoinfo[2]));

}

// custom ControlBehavior
class TimedEvent extends ControlBehavior {
  
  //To work with it in program:
  //  TimedEvent t = (TimedEvent)cp5.getController("myChart").getBehavior();
  ServoInfo servoInf;

  long myTime;
  Chart myChart;
  ChartAxis myAxis;
  
  public int interval;
  private String dataSet;

  boolean update_values = true;

  public TimedEvent(String dataSet_, String chart_, ChartAxis chartAxis, ServoInfo servo) { 

    dataSet = dataSet_;
    servoInf = servo;

    if (servoInf == null) println("SERVOINFO IS NULL");
    
    myChart = (Chart)cp5.getController(chart_);
    interval = (int)((chartAxis.total_time * 1000) / (float)myChart.getDataSet(dataSet_).size());
    reset();

  }

  void reset() { 

    myTime = millis() + interval;

  }
  
  public void update() {
    if (update_values && millis() > myTime) {
      myChart.unshift(dataSet, -servoInf.angle); 
      reset();
    }
  }
}

void draw_servos() {

  //--FOLLOW MOUSE--//
  // angle1 = degrees(atan2(mouseY-(height*0.25), (mouseX-(width * 0.15)-54)));
  // angle2 = degrees(atan2(mouseY-(height*0.5), (mouseX-(width * 0.15)-54)));
  // angle3 = degrees(atan2(mouseY-(height*0.75), (mouseX-(width * 0.15)-54)));

  angle1 = servoinfo[0].angle;
  angle2 = servoinfo[1].angle;
  angle3 = servoinfo[2].angle;

  //--DRAW SERVO 1--//
  pushMatrix();
  translate(width * 0.15, height*0.25);
  shape(ServoBody, - 320*scale_servo_draw, - 123*scale_servo_draw);
  translate(54, 0);
  rotate(radians(179.5 - angle1));
  shape(ServoHorn, - 189*scale_horn_draw, - 560*scale_horn_draw);
  popMatrix();

  //--DRAW SERVO 2--//
  pushMatrix();
  translate(width * 0.15, height*0.5);
  shape(ServoBody, - 320*scale_servo_draw, - 123*scale_servo_draw);
  translate(54, 0);
  rotate(radians(179.5 - angle2));
  shape(ServoHorn, - 189*scale_horn_draw, - 560*scale_horn_draw);
  popMatrix();

  //--DRAW SERVO 3--//
  pushMatrix();
  translate(width * 0.15, height*0.75);
  shape(ServoBody, - 320*scale_servo_draw, - 123*scale_servo_draw);
  translate(54, 0);
  rotate(radians(179.5 - angle3));
  shape(ServoHorn, - 189*scale_horn_draw, - 560*scale_horn_draw);
  popMatrix();
}

void chart_init() {


  int  s1_graph_ypos = (int)(height*0.25 - 75);
  int  s1_graph_xpos = (int)(width * 0.3);

  myservoChart1 = cp5.addChart("servo1")
                  .setPosition(s1_graph_xpos, s1_graph_ypos)
                  .setSize((int)(width*0.5), 150)
                  .setRange(0, 180)
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
                      .setTotalTime(10);
                      ;

  myservoBox1 = new ChartBox(cp5, myservoChart1, "servo1")
                .setFontSize(14)
                .addItem("DC:", String.valueOf(512))
                .addItem("θ:", nf(180,3,1))
                .assignItemValue("DC:", String.valueOf(10))
                .setBarHeight(18); 
                ;

  myservoChart2 = cp5.addChart("servo2")
                  .setPosition(width * 0.3, height*0.5 - 75)
                  .setSize((int)(width*0.5), 150)
                  .setRange(0, 180)
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
                      .setTotalTime(10);
                      ;

  myservoBox2 = new ChartBox(cp5, myservoChart2, "servo2")
                .setFontSize(14)
                .addItem("DC:", String.valueOf(512))
                .addItem("θ:", nf(180,3,1))
                .assignItemValue("DC:", String.valueOf(10))
                .setBarHeight(18); 
                ;

  myservoChart3 = cp5.addChart("servo3")
                  .setPosition(width * 0.3, height*0.75 - 75)
                  .setSize((int)(width*0.5), 150)
                  .setRange(0, 180)
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
                      .setTotalTime(10);
                      ;

  myservoBox3 = new ChartBox(cp5, myservoChart3, "servo3")
                .setFontSize(14)
                .addItem("DC:", String.valueOf(512))
                .addItem("θ:", nf(180,3,1))
                .assignItemValue("DC:", String.valueOf(10))
                .setBarHeight(18); 
                ;
}

void StopServo1Chart(boolean theFlag) {

  TimedEvent t = (TimedEvent)cp5.getController("servo1").getBehavior();
  if (t == null) return;

  if (theFlag) {
    t.update_values = true;
  }
  else {
    t.update_values = false;
  }

} 

void StopServo2Chart(boolean theFlag) {

  TimedEvent t = (TimedEvent)cp5.getController("servo2").getBehavior();
  if (t == null) return;

  if (theFlag) {
    t.update_values = true;
  }
  else {
    t.update_values = false;
  }

} 

void StopServo3Chart(boolean theFlag) {

  TimedEvent t = (TimedEvent)cp5.getController("servo3").getBehavior();
  if (t == null) return;

  if (theFlag) {
    t.update_values = true;
  }
  else {
    t.update_values = false;
  }

} 