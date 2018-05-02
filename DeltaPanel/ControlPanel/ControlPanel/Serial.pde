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

static final int SEND_ANGLES = 20;
static final int SEND_ANGLES_EFPOS = 21;

static final int SERIAL_BUFFER_LEN = 10;
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

    return;
  }

  int command_num = chars_to_int('0', command.charAt(1), command.charAt(2));
  
  switch (command_num) {
    case SEND_ANGLES:       serial_send_angles(); 
                            break;
    case END_OF_STREAM:     serial_next_instruction();
                            break;

    default:
      break;
  } 
}

void serial_next_instruction() {

  switch(serial_mode) {
    case SERVOS_TAB:
      serial_send_header(SEND_ANGLES, true);
      serial_send_header(END_OF_STREAM, true);
      break;
    default:
      break;

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