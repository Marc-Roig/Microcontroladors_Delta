import processing.serial.*;

//--COMUNICAITON--//

Serial myPort;
Buffer buffer;


//CONSTANTS
//--CANVAS PARAMETERS--//
int scale = 10;
float angX = 0;
float angY = 0;

float centerX = 800/2;
float centerY = 800/2 - 200;

boolean rotatePlane = false;
boolean translatePlane = false;

PVector upper[] = new PVector[3]; //Absolute position of rf join with upper plate
PVector lower[] = new PVector[3]; //Absolute position of intersection between rf and re

float[] arms_angles = new float[3];

//--MOVEMENT PARAMETERS--//

PVector cell_selected = new PVector();

float x_pos = 50;
float z_pos = 170; 
float y_pos = 0;

float last_x = x_pos;
float last_y = y_pos;
float last_z = z_pos;

float r = 60;
float angle = 0;

boolean status = true;
float[][] angles_limits = new float[1000][2];
int z_start = 127;
int z_end = 250;

//--BOARD--//

boolean move_endefector = false;

Board board;



void setup() {
  size(800, 800, P3D);
  fill(0);
  stroke(255);

  for (int i = 0; i < 3; i ++) {
    upper[i] = new PVector();
    lower[i] = new PVector();
  }
  
  board = new Board();

  arms_angles[0] = 20;
  arms_angles[1] = 20;
  arms_angles[2] = 20;
  
  //get_limits();
  
  cell_selected.set(-1,-1,0);
  
  //
  choosePort();
  buffer = new Buffer();
}



void draw() {
  
  if (!noPorts_available) serial_communication();
  
  background(0);
  
  //test_circular_movement();
  
  workspace_logic(); //Translations, rotations, scaling...

  status = delta_calcInverse(x_pos, -y_pos, z_pos, arms_angles);
  if(!status) restore_positions();
  
  board.find_selected_cell();
  board.draw_selected_cell();
  board.move_to_selected_cell();

  
  // draw_vertices();
  //check_mouse_position();
  draw_structure();
}

void workspace_logic() {
  //--Move Axis With Mouse--//
  if (rotatePlane) {
    float angY_inc = (mouseX - pmouseX)*2*PI/ width;
    angY += angY_inc;

    float angX_inc = -(mouseY - pmouseY)*PI/ height/2;
    if (abs(angX + angX_inc) < PI/2 ) angX += angX_inc;
  }
  if (translatePlane) {
    float x_inc = (mouseX - pmouseX);
    float y_inc = (mouseY - pmouseY);

    centerX += x_inc;
    centerY += y_inc;
  }
}

void apply_transformations() {
  translate(centerX, centerY);

  //--Scaling--//
  //scale(scale_ratio);

  //--Rotate Axis--//
  rotateX(angX); 
  rotateY(angY);
}

void mousePressed() {
  
  move_endefector = true;
  
  if (mouseButton == RIGHT) {
    translatePlane = true;
  } else if (mouseButton == LEFT) {
    rotatePlane = true;
  }
}

void mouseReleased() {
  if (mouseButton == LEFT) {
    rotatePlane = false;
  }
  if (mouseButton == RIGHT) {
    translatePlane = false;
  }
}

  //println(sqrt(pow(upper[0].x-lower[0].x,2) + pow(upper[0].y-lower[0].y,2) + pow(upper[0].z-lower[0].z,2)));