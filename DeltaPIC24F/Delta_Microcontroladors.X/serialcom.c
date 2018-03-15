#include "main.h"
#include <stdbool.h>
#include "xc.h"
#include <p24fj128ga010.h>

volatile bool command_recieved = false;

volatile Buffer RX_buffer;
volatile Buffer TX_buffer;

void init_UART() {

	U2MODEbits.UARTEN = 1; //Enable UART2

	U2STAbits.UTXISEL1 = 0; //Interrupt when any char is transfered
	U2STAbits.URXISEL1 = 0; //Interrupt flag bit is set when a character is recieved 

	U2STAbits.UTXEN = 1; //Enable UART2 transmiter
//	U2STAbits.URXEN = 1; //Enable UART2 reciever 

	U2BRG = 25; //9600 BAUDRATE

	_U2TXIF = 0;
	_U2TXIE = 1;

	_U2RXIF = 0;
	_U2RXIE = 1;

}

void _ISR _U2TXInterrupt() {

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

void serial_write() {
	
}

void init_buffer(Buffer* buffer) {

	buffer->start = 0;
	buffer->end_ = 0;
	buffer->full = false;
	buffer->empty = true;

}

void serial_push_character(char incomingByte) {

  static int i = 0; //Number of char recieved from current command
  
  if (RX_buffer.full) {
  }
  else {

    RX_buffer.command[RX_buffer.end_][i] = incomingByte;
    i++;
    
    if (incomingByte == '\n') {
      RX_buffer.command_len[RX_buffer.end_] = i; 

      i = 0;

      if (RX_buffer.command[RX_buffer.end_][2] == (END_OF_STREAM - '0')) {
        command_recieved = true;
      }

      inc_buffer_end_pointer(&RX_buffer);

    }
  }

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