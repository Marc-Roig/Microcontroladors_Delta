.include "p24fj128ga010.inc"

.global _init_T4_Interrupt
.global _init_T4
.global _delay

_init_T4_Interrupt:

	RETURN

_init_T4:
	
	PUSH W0

	MOV #0X8030, W0
	MOV W0, T4CON

	POP W0
	RETURN

_delay:

	;W0 a1 - long a
	;W1 a2

	PUSH W2
	PUSH W3

	MOV #1000, W2
	MOV #0, W3

	RCALL _mul_longs

	MOV #16, W2
	MOV #0, W3

	RCALL _div_longs

	;TMR4 is a 16 bits register
	;so from here I work with 16 bit registers

	CLR TMR4

	delay_loop:
	CP TMR4 ;TMR4 - W0 ==> TMR4 - delay_value
	BRA LT, delay_loop

	POP W3
	POP W2

	RETURN

