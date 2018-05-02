//SERIAL SIMULADOR MODE
static final int SERVOS_TAB = 0;

static final int SEND_MODE = 1;
static final int RECIEVE_MODE = 2;
static final int WAIT_MODE = 3;
static final int ASKING_TO_SEND = 4;

//RECIEVER MODE

//GCODE COMMANDS
// static final int ASKED_TO_LISTEN = 0;
// static final int PROCEED = 1;
static final int TRANSMISSION_ENDED = 2; 
static final int MOVE_SERVOS = 3;
static final int CHANGE_SPEED = 4;
static final int MOVE_EF = 5;
static final int END_OF_STREAM = 6;
static final int SEND_MORE = 7;
static final int ASK_TO_LISTEN = 8;
static final int EMERGENCY_STOP = 9;
static final int NEW_DC_VALUES = 10;

static final int SEND_ANGLES = 20;
static final int SEND_DC = 21;
static final int SEND_ANGLES_EFPOS = 22;

static final int SERIAL_BUFFER_LEN = 15;
// static final int SERIAL_COMMAND_MAX_LEN = 50;

boolean noPorts_available = true;

void choosePort() {
   
   printArray(Serial.list() != null);

   try {
     myPort = new Serial(this, Serial.list()[0], 9600);
     noPorts_available = false;
   }
   catch (ArrayIndexOutOfBoundsException e) {
     println("no ports Available!");
   }
}

public class Buffer {
  String[] command;
  int start, end;
  boolean full, empty;
  
  Buffer() {
    command = new String[SERIAL_BUFFER_LEN];
    start = 0;
    end = 0;
    full = false;
    empty = true;
    
    for (int i = 0; i < SERIAL_BUFFER_LEN; i++) {
      command[i] = "";
    }
  }
  
  public boolean inc_end_pointer(){
    end = (end + 1) % SERIAL_BUFFER_LEN;
    empty = false;
    if (end == start) {
      full = true;
      return true;
    }
    return false;
  }
  
  public boolean inc_start_pointer(){
    command[start] = ""; // Erase command

    start = (start + 1) % SERIAL_BUFFER_LEN;
    full = false;

    if (end == start) {
      empty = true;
      return true;
    }
    return false;
  }
  
}

int serial_mode = SERVOS_TAB;

boolean command_recieved = false;

void init_communication() {

  println("Starting Communication with Arduino");

  serial_send_header(END_OF_STREAM, true);

}

void serial_communication(){
       
  if(command_recieved) {
      parse_command(buffer.command[buffer.start]);
      
      if (buffer.inc_start_pointer()) command_recieved = false; //Empty = All stream has been read 
      
  }
  else check_serial(); //Read serial until stream of commands is fully send (G06)   
}

void check_serial() {

  char incomingByte;

  if (myPort.available() > 0) {
    
    incomingByte = myPort.readChar();
    // println(incomingByte);

    if (incomingByte == '\0') return;

    buffer.command[buffer.end] += Character.toString(incomingByte);

    if (incomingByte == '\n'){
      print(buffer.command[buffer.end]);
      if(buffer.command[buffer.end].charAt(2) == '6') {
        command_recieved = true;
      }

      buffer.inc_end_pointer();
    }
  }

}

void parse_command(String command) {

  if (command.charAt(0) != 'G') {
    // write_console(command);
    // println(command);
    return;
  }

  int command_num = chars_to_int('0', command.charAt(1), command.charAt(2));
  
  switch (command_num) {
    case SEND_ANGLES:       serial_send_angles(); 
                            break;

    case END_OF_STREAM:     serial_next_instruction();
                            break;

    case MOVE_SERVOS:       update_angles(command);
                            break;

    case NEW_DC_VALUES:     update_dc(command);
                            break;
                            
    default:                serial_send_header(END_OF_STREAM, true);
                            break;
  } 
}

void serial_next_instruction() {

  switch(serial_mode) {
    case SERVOS_TAB:
      serial_send_header(SEND_ANGLES, true);
      serial_send_header(SEND_DC, true);
      serial_send_header(END_OF_STREAM, true);
      break;
    default:
      break;

  }

}
void update_angles(String command) {

  //G03 090 112 001
  float last_angle0 = servoinfo[0].angle;
  float last_angle1 = servoinfo[1].angle;
  float last_angle2 = servoinfo[2].angle;

  try {

    servoinfo[0].angle = chars_to_int(command.charAt(4), command.charAt(5), command.charAt(6));
    servoinfo[1].angle = chars_to_int(command.charAt(8), command.charAt(9), command.charAt(10));
    servoinfo[2].angle = chars_to_int(command.charAt(12), command.charAt(13), command.charAt(14));

    myservoBox1.assignItemValue("θ:", nf(servoinfo[0].angle, 3, 0));
    myservoBox2.assignItemValue("θ:", nf(servoinfo[1].angle, 3, 0));
    myservoBox3.assignItemValue("θ:", nf(servoinfo[2].angle, 3, 0));

  } catch(ArrayIndexOutOfBoundsException excepcion) {

    println("UPDATE ANGLES -> Array out of Bounds");

    servoinfo[0].angle = last_angle0;
    servoinfo[1].angle = last_angle1;
    servoinfo[2].angle = last_angle2;

  }

}

void update_dc(String command) {

  //G10 1090 1112 1001
  int last_duty_cycle0 = servoinfo[0].duty_cycle;
  int last_duty_cycle1 = servoinfo[1].duty_cycle;
  int last_duty_cycle2 = servoinfo[2].duty_cycle;

  try {

    servoinfo[0].duty_cycle = chars_to_int_4digits(command.charAt(4), command.charAt(5), command.charAt(6), command.charAt(7));
    servoinfo[1].duty_cycle = chars_to_int_4digits(command.charAt(9), command.charAt(10), command.charAt(11), command.charAt(12));
    servoinfo[2].duty_cycle = chars_to_int_4digits(command.charAt(14), command.charAt(15), command.charAt(16), command.charAt(17));

    myservoBox1.assignItemValue("DC:", nf(servoinfo[0].duty_cycle, 3, 0));
    myservoBox2.assignItemValue("DC:", nf(servoinfo[1].duty_cycle, 3, 0));
    myservoBox3.assignItemValue("DC:", nf(servoinfo[2].duty_cycle, 3, 0));

  } catch(ArrayIndexOutOfBoundsException excepcion) {

    println("UPDATE ANGLES -> Array out of Bounds");

    servoinfo[0].duty_cycle = last_duty_cycle0;
    servoinfo[1].duty_cycle = last_duty_cycle1;
    servoinfo[2].duty_cycle = last_duty_cycle2;

  }

}

void serial_send_header(int command_num, boolean end_with_new_line) {

  myPort.write("G" + int_to_string_2digits(command_num));
  if (end_with_new_line) myPort.write("\n");
  else myPort.write(" ");
  
}

void serial_send_angles() {
  //String firstAngle = int_to_string((int)arms_angles[0]+90);
  //String secondAngle = int_to_string((int)arms_angles[1]+90);
  //String thirdAngle = int_to_string((int)arms_angles[2]+90);

  //myPort.write("G03 ");
  //myPort.write(firstAngle + " ");
  //myPort.write(secondAngle + " ");
  //myPort.write(thirdAngle + "\n");

  //println("Sending G03 " + firstAngle + " " + secondAngle + " " + thirdAngle);

}

String int_to_string_2digits(int numb){
  return String.format("%02d", numb);  
}

String int_to_string(int numb){
  return String.format("%03d", numb);  
}

int chars_to_int(char a, char b, char c){
  int first = (int)a-48;
  int second = (int)b-48;
  int third = (int)c-48;
  
  return (first*100 + second*10 + third);
}

int chars_to_int_4digits(char a, char b, char c, char d){
  int first = (int)a-48;
  int second = (int)b-48;
  int third = (int)c-48;
  int fourth = (int)d-48;
  
  return (first*1000 + second*100 + third*10 + fourth);
}