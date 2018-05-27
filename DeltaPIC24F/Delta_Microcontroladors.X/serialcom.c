#include "Config.h"

volatile SerialBuffer RX_buffer;
volatile SerialBuffer TX_buffer;

volatile bool Serial_busy = false; //True if sending data through TX

void Serial_begin(int baudrate) { 

    U2MODE = 0x8000; //Enable Uart

    U2STA = 0x1400; //Interrupt when any character is in buffer
                    //Recieve is enabled
                    //Transmit is enabled

    U2BRG = 25; //9600 BAUDRATE

    init_RXbuffer();
    init_TXbuffer();
    
    _U2TXIF = 0;
    _U2TXIE = 1; //Enable TX interrupt

    _U2RXIF = 0;
    _U2RXIE = 1; //Enable RX interrupt

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

        Serial_push_character(U2RXREG);

    }

    _U2RXIF = 0;

}

void Serial_push_character(char incomingByte) {
  
  if (RX_buffer.full) {} //Do nothing
  else {
    RX_buffer.command[RX_buffer.end_] = incomingByte;

    inc_RXbuffer_end_pointer();

  }

}

void Serial_write(char data_to_print[]) {


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
                        //of the TXbuffer it will be sent twice.

    }

}

char Serial_read() {

    if (RX_buffer.empty) return '\0';

    char toReturnChar = RX_buffer.command[RX_buffer.start];
    inc_RXbuffer_start_pointer();

    return toReturnChar;

}

int Serial_available() {

    if (RX_buffer.empty) return 0;
    return 1;

}

void Serial_println(int value) {

  char value_to_string[10];
  if (value < 0 ) {
      Serial_write("-");
      value *= -1;
  }
  int_to_char(value, value_to_string);
  Serial_write(value_to_string);
  Serial_write("\n");

}

void Serial_print(int value) {

  char value_to_string[7];
  int_to_char(value, value_to_string);
  Serial_write(value_to_string);

}


//---SERIAL BUFFER---//
void init_RXbuffer() {

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

bool inc_RXbuffer_end_pointer() {

  RX_buffer.end_ = (RX_buffer.end_ + 1) % SERIAL_BUFFER_LEN;
  RX_buffer.empty = false;
  if (RX_buffer.end_ == RX_buffer.start) {
    RX_buffer.full = true;
    return true;   
  }
  return false;

}

bool inc_TXbuffer_end_pointer() {

  TX_buffer.end_ = (TX_buffer.end_ + 1) % SERIAL_BUFFER_LEN;
  TX_buffer.empty = false;
  if (TX_buffer.end_ == TX_buffer.start) {
    TX_buffer.full = true;
    return true;   
  }
  return false;

}

bool inc_RXbuffer_start_pointer() {

  RX_buffer.start = (RX_buffer.start + 1) % SERIAL_BUFFER_LEN;
  RX_buffer.full = false;

  if (RX_buffer.end_ == RX_buffer.start) {
    RX_buffer.empty = true;
    return true;
  }
  return false;

}

bool inc_TXbuffer_start_pointer() {

  TX_buffer.start = (TX_buffer.start + 1) % SERIAL_BUFFER_LEN;
  TX_buffer.full = false;

  if (TX_buffer.end_ == TX_buffer.start) {
    TX_buffer.empty = true;
    return true;
  }
  return false;

}


