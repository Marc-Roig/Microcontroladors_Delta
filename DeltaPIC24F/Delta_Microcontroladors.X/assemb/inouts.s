.include "p24fj128ga010.inc"
.include "assembly_routines.inc"

.extern _RB3_Analog_Active
.extern _RB8_Analog_Active
.extern _RB9_Analog_Active

.extern _RB3_Analog_Value
.extern _RB8_Analog_Value
.extern _RB9_Analog_Value

/*********************************************************************
* Function: _pinMode
*
* Overview: Set a pin to digital input, digital output, analog input
*
*			OUTPUT 0
*			INPUT 1
*			ANALOG_INPUT 2
*           
* PreCondition: none
*
* Input: int pin_num - W0 - pin number (numeration at inouts.h)
*		 int mode 	 - W1 - set as digital input, output, analog input
*
* Output: none
*
********************************************************************/

_pinMode:
	; int pin_num at W0
	; int mode at W1

	CP W1, #3 ; check if modes are valid, there are only 3 (0 to 2)
	BRA GE, END

	ANALOG:

		CP W1, #2 ;if mode == 2 its analog
		BRA NZ, DIGITAL

		CP W0, #3	;only RB3, RB8 and RB9 can be analog
		BRA GE, END

		SL W0, W0 ; pin_num * 2 -> num of lines to jump
		ADD ANALOG_INPUT, W0, W0
		BRA W0

	DIGITAL:

		CP W0, #7 ; Check if input is valid, there are only 7 pins (0 to 6)
		BRA GE, END

		CP W1, #1 ; OUTPUT = 0
		BRA NZ, IS_OUTPUT

		SL W0, W0 ; pin_num * 2 -> num of lines to jump
		ADD DIGITAL_INPUT, W0, W0
		BRA W0

	IS_OUTPUT:

		SL W0, W0 ; pin_num * 2 -> num of lines to jump
		ADD DIGITAL_OUTPUT, W0, W0
		BRA W0


	DIGITAL_INPUT:

		BSET TRISB, #3
		BRA END
		BSET TRISB, #8
		BRA END
		BSET TRISB, #9
		BRA END
		BSET TRISE, #8
		BRA END
		BSET TRISE, #9
		BRA END
		BSET TRISA, #14
		BRA END
		BSET TRISA, #15
		BRA END

	DIGITAL_OUTPUT:

		BCLR TRISB, #3
		BRA END
		BCLR TRISB, #8
		BRA END
		BCLR TRISB, #9
		BRA END
		BCLR TRISE, #8
		BRA END
		BCLR TRISE, #9
		BRA END
		BCLR TRISA, #14
		BRA END
		BCLR TRISA, #15
		BRA END

	ANALOG_INPUT:

		BSET _RB3_Analog_Active, 0
		BRA END
		BSET _RB8_Analog_Active, 0
		BRA END
		BSET _RB9_Analog_Active, 0
		BRA END

	END:

	RETURN

	;increment PC
	;better way to write this function
	;how to make a function return a value to c code


/*********************************************************************
* Function: _digitalRead:
*
* Overview: Read state of a pin previously set as digital input.
*           
* PreCondition: Set pin as digital input in pinMode
*
* Input: int pin_num - W0 - pin number (numeration at inouts.h)
*
* Output: bool pin_state - W0 - state of the required pin
*
********************************************************************/

_digitalRead:

	CHECK_RB3:
		CP W0, #0
		BRA NZ, CHECK_RB8
		MOV _RB3, W0
		BRA END_DIGITAL_READ

	CHECK_RB8:
		CP W0, #1
		BRA NZ, CHECK_RB9
		MOV _RB8, W0
		BRA END_DIGITAL_READ

	CHECK_RB9:
		CP W0, #2
		BRA NZ, CHECK_RE8
		MOV _RB9, W0
		BRA END_DIGITAL_READ

	CHECK_RE8:
		CP W0, #3
		BRA NZ, CHECK_RE9
		MOV _RE8, W0
		BRA END_DIGITAL_READ

	CHECK_RE9:
		CP W0, #4
		BRA NZ, CHECK_A14
		MOV _RE9, W0
		BRA END_DIGITAL_READ

	CHECK_RA14:
		CP W0, #5
		BRA NZ, CHECK_A15
		MOV _RA14, W0
		BRA END_DIGITAL_READ

	CHECK_RA15:
		CP W0, #6
		BRA NZ, INVALID_PIN
		MOV _RA15, W0
		BRA END_DIGITAL_READ

	INVALID_PIN:
		MOV #0, W0

	END_DIGITAL_READ:

	RETURN

/*********************************************************************
* Function: _analogRead:
*
* Overview: Read value of a pin previously set as analog input.
*			Only RB3, RB8 and RB9 can be set as analog input in 
*			pinMode().
*           
* PreCondition: Set pin as analog input in pinMode().
*
* Input: int pin_num - W0 - pin number (numeration at inouts.h)
*
* Output: bool pin_state - W0 - state of the required pin
*
********************************************************************/

 _analogRead:

	;W0 int pin_num

	CP W0, #3
	BRA GE, End_analogRead
	
	SL W0, W0, W0;Jump 2 lines for every case
	BRA W0 ; PC += (W0*2)

	IO_RB3_AR:

		MOV _RB3_Analog_Value, W0
		BRA End_analogRead

	IO_RB8_AR:

		MOV _RB8_Analog_Value, W0
		BRA End_analogRead

	IO_RB9_AR:

		MOV _RB9_Analog_Value, W0
		BRA End_analogRead

	End_analogRead:

	RETURN

    
/*********************************************************************
* Function: _map2:
*
* Overview: Re-maps a number from one range to another. That is, a value of
*			fromLow would get mapped to toLow, a value of fromHigh to toHigh, 
*			values in-between to values in-between, etc. Does not constrain 
*			values to within the range, because out-of-range values are sometimes 
*			intended and useful.
*
*			Used c code from Arduino reference page:
*			https://www.arduino.cc/reference/en/language/functions/math/map/
*           
* PreCondition: In assembly all parameters have to be pushed into the stack before
*				calling this function in the following order
*
* Input: long x - [W14-26] - value to remap
*		 long in_min - [W14-22] - the lower bound of the value's current range
*		 long in_max - [W14-16] - the upper bound of the value's current range
*		 long out_min - [W14-14] - the lower bound of the value's target range
*		 long out_max - [W14-10] - the upper bound of the value's target range
*
* Output: long mapped_value - W0, W1 - The mapped value
*
********************************************************************/

_map2: ;CHECKED (NOT FOR NEGATIVE NUMBERS)

	LNK #4 ;Save two positions in stack to save intermediate values

	PUSH W2 ;W2 and W3 are modified in this routine
	PUSH W3

	MOV [W14-26], W0 ; x lower word
	MOV [W14-24], W1 ; x upper word
	MOV [W14-22], W2 ; in_min lower word
	MOV [W14-20], W3 ; in_min upper word

	RCALL _sub_longs ; (x - in_min)

	MOV.D W0, [W14] ; Store value in stack

	MOV [W14-10], W0 ; out_max lower word
	MOV [W14-8], W1  ; out_max upper_word
	MOV [W14-14], W2 ; out_min lower_word
	MOV [W14-12], W3 ; out_min upper_word

	RCALL _sub_longs ;(out_max - out_min) 

	MOV [W14], W2 
	MOV [W14+2], W3

	RCALL _mul_longs ;(x - in_min) * (out_max - out_min) 

	MOV.D W0, [W14] 

	MOV [W14-18], W0 ; in_max lower word 
	MOV [W14-16], W1 ; in_max upper word
	MOV [W14-22], W2 ; in_min lower word
	MOV [W14-20], W3 ; in_min upper word

	RCALL _sub_longs ; (in_max - in_min)

	MOV W0, W2 ;move result of substraction to w2 and w3
	MOV W1, W3

	MOV [W14], W0 
	MOV [W14+2], W1 ; result of (x - in_min) * (out_max - out_min) 

	RCALL _div_longs ; (x - in_min) * (out_max - out_min) / (in_max - in_min)

	MOV [W14-14], W2 ; out_min lower word
	MOV [W14-12], W3 ; out_min upper word

	RCALL _add_longs ; (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

	POP W3
	POP W2
	
	ULNK
	
	RETURN