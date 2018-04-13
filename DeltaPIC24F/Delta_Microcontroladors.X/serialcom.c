#include "main.h"

volatile bool command_recieved = false;

volatile Buffer RX_buffer;
volatile TXBuffer TX_buffer;

volatile bool Serial_busy = false; //True if sending data through TX

volatile int serial_mode = 0;

void Serial_begin(int baudrate_) { 

    U2MODE = 0x8000;

    U2STA = 0x1400;

    U2BRG = 25; //9600 BAUDRATE

    _U2TXIF = 0;
    _U2TXIE = 1; //Enable TX interrupt

    _U2RXIF = 0;
    _U2RXIE = 1; //Enable RX interrupt

    init_buffer();
    init_TXbuffer();

}

void _ISR _U2TXInterrupt() {

    if(!TX_buffer.empty)  {

        U2TXREG = TX_buffer.command[TX_buffer.start];
        inc_TXbuffer_start_pointer();

    } else Serial_busy = false;

    _U2TXIF = 0;

}

void _ISR _U2RXInterrupt() {

    if (U2STAbits.URXDA) { //what if there are more bits in the buffer??

        serial_push_character(U2RXREG);

    }

    _U2RXIF = 0;

}

void serial_write(char data_to_print[]) {


    int data_size = strlength(data_to_print);

    if (TX_buffer.full) return;

    int i;
    for (i = 0 ; i < data_size; i++) {

        TX_buffer.command[TX_buffer.end_] = data_to_print[i];
        if (inc_TXbuffer_end_pointer()) break;

    }

    if (!Serial_busy) {

        Serial_busy = true;
        // U2TXREG = TX_buffer.command[TX_buffer.start];
        // inc_TXbuffer_start_pointer();
        U2TXREG = '\0'; //Send a dummy character to start the serial write
                        //Still dont know why if sent here the first char
                        //of the TXbuffer it will be printed out twice.

    }

    
}

void serial_println(int value) {

  char value_to_string[7];
  int_to_char(value, value_to_string);
  serial_write(value_to_string);
  serial_write("\r\n");

}

void serial_print(int value) {

  char value_to_string[7];
  int_to_char(value, value_to_string);
  serial_write(value_to_string);

}

//---COMMANDS--//

void parse_command(char command[SERIAL_COMMAND_MAX_LEN]) {

  int command_num = chars_to_int('0', command[1], command[2]);
  switch(command_num) {
    case END_OF_STREAM: serial_next_instruction();
                        break;
      
    case MOVE_SERVOS:   serial_recieve_angles(command);
                        break;

    default:            serial_write("BAD REQUEST");
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

void send_command_header(int command_num, bool end_with_new_line) {

  serial_write("G");
  serial_write(int_to_char_2digits(command_num));
  if (end_with_new_line) serial_write("\n");
  else serial_write(" ");

}

//---SEND DATA---//
void serial_send_angles() {

  send_command_header(MOVE_SERVOS, false);
  int i;
  for (i = 0; i < 3; i++) {
  // serial_write(int_to_char_3digits(servos_angles[i]));
    
    if (i == 2) serial_write("\n");
    else serial_write(" "); 
  }

}

//---DATA PARSING---//
void serial_push_character(char incomingByte) {

  static int i = 0; //Number of char recieved from current command
  
  if (RX_buffer.full) { //Code something to manage the buffer when full
  }
  else {

    RX_buffer.command[RX_buffer.end_][i] = incomingByte;
    i++;
    
    if (incomingByte == '\n') {
      RX_buffer.command_len[RX_buffer.end_] = i; 

      i = 0;

      if (RX_buffer.command[RX_buffer.end_][2] == (END_OF_STREAM + '0')) {
        command_recieved = true;
      }

      inc_buffer_end_pointer(&RX_buffer);

    }
  }

}

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
  else; //serial_write("BAD REQUEST\n");

}

//---SERIAL BUFFER---//
void init_buffer() {

  RX_buffer.start = 0;
  RX_buffer.end_ = 0;
  RX_buffer.full = false;
  RX_buffer.empty = true;

}

void init_TXbuffer() {

  TX_buffer.start = 0;
  TX_buffer.end_ = 0;
  TX_buffer.full = false;
  TX_buffer.empty = true;

}

bool inc_buffer_end_pointer() {

  RX_buffer.end_ = (RX_buffer.end_ + 1) % SERIAL_BUFFER_LEN;
  RX_buffer.empty = false;
  if (RX_buffer.end_ == RX_buffer.start) {
    RX_buffer.full = true;
    return true;   
  }
  return false;

}

bool inc_TXbuffer_end_pointer() {

  TX_buffer.end_ = (TX_buffer.end_ + 1) % TX_SERIAL_BUFFER_LEN;
  TX_buffer.empty = false;
  if (TX_buffer.end_ == TX_buffer.start) {
    TX_buffer.full = true;
    return true;   
  }
  return false;

}

bool inc_buffer_start_pointer() {

  RX_buffer.start = (RX_buffer.start + 1) % SERIAL_BUFFER_LEN;
  RX_buffer.full = false;

  if (RX_buffer.end_ == RX_buffer.start) {
    RX_buffer.empty = true;
    return true;
  }
  return false;

}

bool inc_TXbuffer_start_pointer() {

  TX_buffer.start = (TX_buffer.start + 1) % TX_SERIAL_BUFFER_LEN;
  TX_buffer.full = false;

  if (TX_buffer.end_ == TX_buffer.start) {
    TX_buffer.empty = true;
    return true;
  }
  return false;

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

