#include "main.h"

volatile bool command_recieved = false;

volatile Buffer RX_buffer;
volatile Buffer TX_buffer;

volatile bool Serial_busy = false; //True if sending data through TX

void init_UART() {

	// U2MODEbits.UARTEN = 1; //Enable UART2

	// U2STAbits.UTXISEL1 = 0; //Interrupt when any char is transfered
	// U2STAbits.URXISEL1 = 0; //Interrupt flag bit is set when a character is recieved 

	// U2STAbits.UTXEN = 1; //Enable UART2 transmiter
	// U2STAbits.URXEN = 1; //Enable UART2 reciever 

	U2MODE = 0x8000;

	U2STA = 0x1400;

	U2BRG = 25; //9600 BAUDRATE

	_U2TXIF = 0;
	_U2TXIE = 1; //Enable TX interrupt

	_U2RXIF = 0;
	_U2RXIE = 1; //Enable RX interrupt

}

void _ISR _U2TXInterrupt() {

  static int i = 1; //Pivot of the sentence, first character already sent.

  char ByteToSend;

  if(!TX_buffer.empty)  {

    ByteToSend = TX_buffer.command[TX_buffer.start][i];

    U2TXREG = ByteToSend;

    i++;

    if (ByteToSend == '\n') {

      i = 0; 
      inc_buffer_start_pointer(&TX_buffer);

    }

  } else {

    Serial_busy = false;
    i = 1;

  }

	_U2TXIF = 0;

}

void _ISR _U2RXInterrupt() {

	char incomingByte;

	if (U2STAbits.URXDA) { //what if there are more bits in the buffer??

		incomingByte = U2RXREG;
		serial_push_character(incomingByte);

	}

	_U2RXIF = 0;

}

void serial_write(char data_to_print[]) {

  static int last_char_position = 0;

  int data_size = sizeof(h) - 1;

  //Prompt lcd error
  if (data_size <= SERIAL_COMMAND_MAX_LEN) return;
  if (TX_buffer.full) return;
  

  for (int i = 0; i < data_size; i++) {
    TX_buffer.command[TX_buffer.end_][i + last_char_position] = data_to_print[i];
  }

  if (data_to_print[data_size-1] == '\n') {

    if (TX_buffer.command[TX_buffer.end_][2] == (END_OF_STREAM + '0') && !Serial_busy) {

      Serial_busy = true;

      U2TXREG = TX_buffer.command[TX_buffer.start][0];

    }

    inc_buffer_end_pointer(&TX_buffer);

    i = 0;
  }
    
}

//---COMMANDS--//

void parse_command(char command[SERIAL_COMMAND_MAX_LEN]) {

  int command_num = chars_to_int('0', command[1], command[2]);
  switch(command_num) {
    case END_OF_STREAM: serial_next_instruction();
                        break;
      
    case MOVE_SERVOS:   serial_recieve_angles(command);
                        break;

    default:            Serial.write("BAD REQUEST");
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

  Serial.write("G");
  Serial.write(int_to_char_2digits(command_num));
  if (end_with_new_line) Serial.write('\n');
  else Serial.write(" ");

}

//---SEND DATA---//
void serial_send_angles() {

  send_command_header(MOVE_SERVOS, false);
  for (int i = 0; i < 3; i++) {
    Serial.write(int_to_char_3digits(servos_angles[i]));
    
    if (i == 2) Serial.write("\n");
    else Serial.write(' '); 
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
  
  for (int i = 4; i < 7; i++) {
    if (!is_alphanumeric(command[i])) bad_request = true; 
  }
  for (int i = 8; i < 11; i++) {
    if (!is_alphanumeric(command[i])) bad_request = true;
  }
  for (int i = 12; i < 15; i++) {
    if (!is_alphanumeric(command[i])) bad_request = true;
  }

  if (!bad_request) {
    servos_angles[0] = chars_to_int(command[4], command[5], command[6]);
    servos_angles[1] = chars_to_int(command[8], command[9], command[10]);
    servos_angles[2] = chars_to_int(command[12], command[13], command[14]);
  }
  else //serial_write("BAD REQUEST\n");

}

//---SERIAL BUFFER---//
void init_buffer(Buffer* buffer) {

  buffer->start = 0;
  buffer->end_ = 0;
  buffer->full = false;
  buffer->empty = true;

}

bool inc_buffer_end_pointer(Buffer* buffer) {

  buffer->end_ = (buffer->end_ + 1) % SERIAL_BUFFER_LEN;
  buffer->empty = false;
  if (buffer->end_ == buffer->start) {
    buffer->full = true;
    return true;   
  }
  return false;

}

bool inc_buffer_start_pointer(Buffer* buffer) {

  buffer->start = (buffer->start + 1) % SERIAL_BUFFER_LEN;
  buffer->full = false;

  if (buffer->end_ == buffer->start) {
    buffer->empty = true;
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

int chars_to_int(char a, char b, char c) {
  
  return (a-'0')*100 + (b-'0')*10 + c-'0';

}

bool is_alphanumeric(char a) {

  if (a > 47 && a < 58) return true;
  return false;

}

