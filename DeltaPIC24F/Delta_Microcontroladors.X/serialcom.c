#include "main.h"

void ini_UART() {

	U2MODEbits.UARTEN = 1;

	U2STAbits.UTXISEL = 0; //Interrupt when any char is transfered
	U2STAbits.URXISEL = 0; //Interrupt flag bit is set when a character is recieved 

	U2STAbits.UTXEN = 1; //UART2 transmiter is enabled
	U2STAbits.URXEN = 1; //UART2 reciever is enabled

	U2BRG = 25;

	_U2TXIF = 0;
	_U2TX1E = 1;

	_U2RXIF = 0;
	_U2RX1E = 1;



}

void _ISR _U2TXInterrupt() {

	_U2TXIF = 0;

}

//REGISTER U1TXREG TO TRANSMIT U1RXREG TO RECIEVE


void _ISR _U2RXInterrupt() {

	char incomingByte;

	if (U2STAbits.UR2DA) { //what if there are more bits in the buffer??
		incomingByte = U2RXREG;
	}

	_U2RXIF = 0;

}
