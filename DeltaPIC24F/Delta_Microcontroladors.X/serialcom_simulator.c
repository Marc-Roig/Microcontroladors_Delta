#include "Config.h"

static volatile bool command_recieved = false;
static volatile int serial_mode = 0;

CommandsBuffer buffer;

void serial_com_with_simulator() { //FUNCITON TO CALL IN MAIN

  if(command_recieved) {
    parse_command(buffer.command[buffer.start]);
    if (inc_buffer_start_pointer()) {
      command_recieved = false; //If stack empty message has been read
      delay(SERIAL_DELAY_MS);
    }
  }
  
  else check_serial();

}

//---COMMANDS--//

void parse_command(char command[SERIAL_COMMAND_MAX_LEN]) {

  int command_num = chars_to_int('0', command[1], command[2]);

  switch(command_num) {

    case END_OF_STREAM: serial_next_instruction();
                        break;
      
    case MOVE_SERVOS:   serial_recieve_angles(command);
                        break;

    default:            Serial_write("BAD REQUEST");
                        break;

  }
  
}

void serial_next_instruction() {

  switch (serial_mode) {

    case ASK_FOR_ANGLES:  send_command_header(SEND_ME_ANGLES, true);
                          send_command_header(END_OF_STREAM, true);
                          break;

    case GIVE_ANGLES:     serial_send_angles();
                          send_command_header(END_OF_STREAM, true);
                          break;

    default:              break;

  }

}

//--RECIEVE DATA--//
void check_serial() {

  static int i = 0; //Number of char recieved from current command
  char incomingByte;
  
  if (buffer.full) {
    //How to get the last one.
    //Get the first one, introduce the G06
    //Wait some time to start again the communication
  }
  else if (Serial_available() > 0 ) {
    incomingByte = Serial_read();

    buffer.command[buffer.end_][i] = incomingByte;
    i++;

    if (incomingByte == '\n') {
      buffer.command_len[buffer.end_] = i; 

      i = 0;

      if (buffer.command[buffer.end_][2] == (END_OF_STREAM + '0')) {
        command_recieved = true;
      }

      inc_buffer_end_pointer();

    }
  }

}

bool inc_buffer_end_pointer() {

  buffer.end_ = (buffer.end_ + 1) % MAX_COMMANDS_NUM;
  buffer.empty = false;
  if (buffer.end_ == buffer.start) {
    buffer.full = true;
    return true;   
  }
  return false;

}

bool inc_buffer_start_pointer() {

  buffer.start = (buffer.start + 1) % MAX_COMMANDS_NUM;
  buffer.full = false;

  if (buffer.end_ == buffer.start) {
    buffer.empty = true;
    return true;
  }
  return false;

}

void init_buffer() {

  buffer.start = 0;
  buffer.end_ = 0;
  buffer.empty = true;
  buffer.full = false;
  
}


void send_command_header(int command_num, bool end_with_new_line) {

  Serial_write("G");
  Serial_write(int_to_char_2digits(command_num));
  if (end_with_new_line) Serial_write("\n");
  else Serial_write(" ");

}

//---SEND DATA---//
void serial_send_angles() {

  send_command_header(MOVE_SERVOS, false);
  int i;
  for (i = 0; i < 3; i++) {
  // Serial_write(int_to_char_3digits(servos_angles[i]));
    
    if (i == 2) Serial_write("\n");
    else Serial_write(" "); 
  }

}

//---DATA PARSING---//


void serial_recieve_angles(char command[SERIAL_COMMAND_MAX_LEN]) {

  bool bad_request = false;
  
  int i;
  
  for (i = 4; i < 7; i++) {
    if (!is_alphanumeric(command[i])) bad_request = true; 
  }
  for (i = 8; i < 11; i++) {
    if (!is_alphanumeric(command[i])) bad_request = true;
  }
  for (i = 12; i < 15; i++) {
    if (!is_alphanumeric(command[i])) bad_request = true;
  }

  if (!bad_request) {
    //servos_angles[0] = chars_to_int(command[4], command[5], command[6]);
    //servos_angles[1] = chars_to_int(command[8], command[9], command[10]);
    //servos_angles[2] = chars_to_int(command[12], command[13], command[14]);
  }
  else; //Serial_write("BAD REQUEST\n");

}


//--UTILITIES--//
char* int_to_char_2digits(int numb) {

  char* sentence = "  ";

  sentence[1] = numb % 10 + '0';
  sentence[0] = numb / 10 % 10 + '0';

  return sentence;

}

char* int_to_char_3digits(int numb) {

  char* sentence = "   ";

  sentence[2] = numb % 10 + '0';
  sentence[1] = numb / 10 % 10 + '0';
  sentence[0] = numb / 100 % 10 + '0';

  return sentence;

}

double power(double base, double expon) {
    
    double result = base;
    
    if (expon == 0) return 1;
    
    int i;
    for (i = 0; i < expon-1; i++) {
        result *= base;
    }
    
    return result;
}

void int_to_char(int number, char* converted_char) {
    
    int number_len = 1;
    int temp = number;

    while(temp > 9) {
        temp /= 10;
        number_len++;
    }

    int i;
    for (i = 0; i < number_len; i++) {

        converted_char[i] = number / ((int)power(10, (number_len-i-1))) % 10 + '0';
        
    }
    
    converted_char[number_len] = '\0';

}


int strlength(char *p) {

        int len = 0;
        while (*p++)
                len++;
        return len;

}

int chars_to_int(char a, char b, char c) {
  
  return (a-'0')*100 + (b-'0')*10 + c-'0';

}

bool is_alphanumeric(char a) {

  if (a > 47 && a < 58) return true;
  return false;

}


void serial_write_dc_every_ms(int wait_time) {

    static unsigned int startMilis = 0;
    
    if ((millis() - startMilis) > wait_time) {

        startMilis = millis();

        Serial_write("Duty cycle: ");
        Serial_print((int)servoinfo[0].duty_cycle);
        Serial_write(" - ");
        Serial_print((int)servoinfo[1].duty_cycle);
        Serial_write(" - ");
        Serial_println(servoinfo[2].duty_cycle);

    }

}

void serial_write_angles() {

    Serial_write("Angles: ");
    Serial_print((int)servoinfo[0].angle);
    Serial_write(" - ");
    Serial_print((int)servoinfo[1].angle);
    Serial_write(" - ");
    Serial_println(servoinfo[2].angle);

}
