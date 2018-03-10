public class Board {
  //---BOARD PARAMETERS---///
  int space_between_lines = 40;
  int num_lines = 8;

  int total_len = space_between_lines * num_lines;
  int origin = -total_len / 2;

  PVector[][] vertices_mapped = new PVector[9][9];

  boolean map_vertices = true;

  //---END EFECTOR MOVEMENT--//
  float desired_xpos = x_pos;
  float desired_ypos = y_pos;

  float speed = 3;

  boolean x_direction = false;
  boolean y_direction = false;

  //CELL SELECTION
  PVector[] cols_lines = new PVector[9];
  PVector[] rows_lines = new PVector[9];

  int pointed_row = -1; //-1 = no cell pointed
  int pointed_col = -1; //-1 = no cell pointed

  Board() {
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        vertices_mapped[i][j] = new PVector();
      }
    }

    for (int i = 0; i < 9; i++) {
      cols_lines[i] = new PVector();
      rows_lines[i] = new PVector();
    }
  }

  public void draw_grid() {

    if (rotatePlane || translatePlane) map_vertices = true;

    pushMatrix();
    rotateX(PI/2);

    translate(origin, origin);

    for (int i = 0; i <= num_lines; i++) {
      line(i * space_between_lines, 0, 0, i * space_between_lines, total_len, 0 );
      stroke(255);
      line(0, i * space_between_lines, 0, total_len, i * space_between_lines, 0);
    }

    if (map_vertices) get_vertices();
    popMatrix();
  }

  private void get_vertices() {
    int x = 0;
    int y = 0;

    pushMatrix();
    for (int i = 0; i <= num_lines; i++) {
      pushMatrix();
      for (int j = 0; j <= num_lines; j++) {
        x = i*space_between_lines;
        y = j*space_between_lines;
        vertices_mapped[i][j].set(screenX(x, y, 0), screenY(x, y, 0), screenZ(x, y, 0));
      }
      popMatrix();
    }
    popMatrix();

    map_vertices = false;
  }

  public void find_selected_cell() {

    PVector[] cols1 = new PVector[9];
    PVector[] rows1 = new PVector[9];
    PVector[] cols2 = new PVector[9];
    PVector[] rows2 = new PVector[9];

    pointed_row = -1;
    pointed_col = -1;

    for (int i = 0; i < 9; i++) { //For an easy understanding of what is what
      rows1[i] = vertices_mapped[0][i];
      rows2[i] = vertices_mapped[8][i];
      cols1[i] = vertices_mapped[i][0];
      cols2[i] = vertices_mapped[i][8];
    }

    float x1;
    float x2;
    float y1;
    float y2;

    float m;
    float n;

    for (int i = 0; i < 9; i++) {  //Get lines
      x1 = cols1[i].x;
      x2 = cols2[i].x;
      y1 = cols1[i].y;
      y2 = cols2[i].y;

      m = (y2-y1)/(x2-x1);
      n = y1 - x1*(y2-y1)/(x2-x1);

      cols_lines[i].set(m, n);

      x1 = rows1[i].x;
      x2 = rows2[i].x;
      y1 = rows1[i].y;
      y2 = rows2[i].y;

      m = (y2-y1)/(x2-x1);
      n = y1 - x1*(y2-y1)/(x2-x1);

      rows_lines[i].set(m, n);
    }

    float y = 0;
    float x = 0;

    for (int i = 0; i < 9; i++) {
      m = rows_lines[i].x;
      n = rows_lines[i].y;

      y = m*mouseX + n;
      if (mouseY < y) {
        //println(y, mouseX);
        break;
      }
      pointed_row++;
    }

    for (int i = 0; i < 9; i++) {
      m = cols_lines[i].x;
      n = cols_lines[i].y;

      x = (mouseY-n)/m;
      if ((mouseX) < x) break;
      pointed_col++;
    }

    cell_selected.set(pointed_col, pointed_row);  
  }

  public void draw_selected_cell() {
    if (pointed_row >= 0 && pointed_row < 8 && pointed_col >= 0 && pointed_col < 8) {

      float a1 = vertices_mapped[pointed_col][pointed_row].x;
      float a2 = vertices_mapped[pointed_col][pointed_row+1].x;
      float a3 = vertices_mapped[pointed_col+1][pointed_row+1].x;
      float a4 = vertices_mapped[pointed_col+1][pointed_row].x;

      float b1 = vertices_mapped[pointed_col][pointed_row].y;
      float b2 = vertices_mapped[pointed_col][pointed_row+1].y;
      float b3 = vertices_mapped[pointed_col+1][pointed_row+1].y;
      float b4 = vertices_mapped[pointed_col+1][pointed_row].y;


      beginShape();
      fill(230, 230, 230, 70);
      vertex(a1, b1);
      vertex(a2, b2);
      vertex(a3, b3);
      vertex(a4, b4);
      endShape(CLOSE);

      fill(0);
    }
  }

  public void move_to_selected_cell() {
    float x = cell_selected.x;
    float y = cell_selected.y; 

    pushMatrix();

    if (x<8 && y <8 && x >= 0 && y >= 0 && !move_endefector) {
      desired_xpos = space_between_lines*(x+0.5) + origin;
      desired_ypos = space_between_lines*(y+0.5) + origin;

      if (desired_xpos < x_pos) x_direction = false;
      else x_direction = true;
      if (desired_ypos < y_pos) y_direction = false;
      else y_direction = true;
    }

    popMatrix();
    back_up_positions();
    if (move_endefector) {
      if (x_pos != desired_xpos) {
        if (x_direction) {
          x_pos += speed;
          if (x_pos >= desired_xpos) x_pos = desired_xpos;
        } else {
          x_pos -= speed;
          if (x_pos <= desired_xpos) x_pos = desired_xpos;
        }
      }
      if (y_pos != desired_ypos) {
        if (y_direction) {
          y_pos += speed;
          if (y_pos > desired_ypos) y_pos = desired_ypos;
        } else {
          y_pos -= speed;
          if (y_pos < desired_ypos) y_pos = desired_ypos;
        }
      }

      if (x_pos == desired_xpos && y_pos == desired_ypos) {
        move_endefector = false;
      }
    }
  }
}

void back_up_positions() {
  last_x = x_pos;
  last_y = y_pos;
  last_z = z_pos;
}

void restore_positions() {
  x_pos = last_x;
  y_pos = last_y;
  z_pos = last_z;
  
  move_endefector = false; // Stop movement
}

void draw_vertices() {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j<9; j++) {
      pushMatrix();
      translate(board.vertices_mapped[i][j].x, board.vertices_mapped[i][j].y, 0);
      sphere(5);
      popMatrix();
    }
  }
}