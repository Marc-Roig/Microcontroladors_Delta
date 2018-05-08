.include "p24fj128ga010.inc"
.include "assembly_routines.inc"

.extern _RB3_Analog_Active
.extern _RB8_Analog_Active
.extern _RB9_Analog_Active

.extern _RB3_Analog_Value
.extern _RB8_Analog_Value
.extern _RB9_Analog_Value


_pinMode:
	; int pin_num at W0
	; int mode at W1

	CP W1, #3 ; check if modes are valid
	BRA GE, END

	ANALOG:

		CP W1, #2
		BRA NZ, DIGITAL

		CP W0, #3	; only RB3, RB8 and RB9 can be analog
		BRA GE, END

		SL W0, W0 ; pin_num * 2 -> num of lines to jump
		ADD ANALOG_INPUT, W0, W0
		RCALL W0

	DIGITAL:

		CP W0, #7 ; Check if input is valid
		BRA GE, END

		CP W1, #1
		BRA NZ, IS_OUTPUT

		SL W0, W0 ; pin_num * 2 -> num of lines to jump
		ADD DIGITAL_INPUT, W0, W0
		RCALL W0

	IS_OUTPUT:

		SL W0, W0 ; pin_num * 2 -> num of lines to jump
		ADD DIGITAL_OUTPUT, W0, W0
		RCALL W0


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

_digitalRead:

	CHECK_RB3:
		CP W0, #0
		BRA NZ, CHECK_RB8:
		MOV _RB3, W0
		BRA END

	CHECK_RB8:
		CP W0, #1
		BRA NZ, CHECK_RB9:
		MOV _RB8, W0
		BRA END

	CHECK_RB9:
		CP W0, #2
		BRA NZ, CHECK_RE8:
		MOV _RB9, W0
		BRA END

	CHECK_RE8:
		CP W0, #3
		BRA NZ, CHECK_RE9:
		MOV _RE8, W0
		BRA END

	CHECK_RE9:
		CP W0, #4
		BRA NZ, CHECK_A14:
		MOV _RE9, W0
		BRA END

	CHECK_RA14:
		CP W0, #5
		BRA NZ, CHECK_A15:
		MOV _RA14, W0
		BRA END

	CHECK_RA15:
		CP W0, #6
		BRA NZ, INVALID_PIN:
		MOV _RA15, W0
		BRA END

	INVALID_PIN:
		MOV #0, W0

	END:

	RETURN

_analogRead:

	;W0 int pin_num

	SL W0 ;Jump 2 lines for every case
	BRA W0 ; PC += (W0*2)

	IO_RB3_AR:

		MOV #_RB3_Analog_Value, W0
		BRA End_analogRead

	IO_RB8_AR:

		MOV #_RB8_Analog_Value, W0
		BRA End_analogRead

	IO_RB9_AR:

		MOV #_RB9_Analog_Value, W0
		BRA End_analogRead

	End_analogRead:

		RETURN

_map2: ;CHECKED (NOT FOR NEGATIVE NUMBERS)
	;All parameters are pushed into stack
	;long x = [W14-26]
	;long in_min = [W14-22]
	;long in_max = [W14-18]
	;long out_min = [W14-14]
	;long out_max = [W14-10]
	LNK #4

	PUSH W2
	PUSH W3

	MOV [W14-26], W0
	MOV [W14-24], W1
	MOV [W14-22], W2
	MOV [W14-20], W3

	RCALL _sub_longs

	MOV.D W0, [W14] ;(x - in_min)

	MOV [W14-10], W0
	MOV [W14-8], W1
	MOV [W14-14], W2
	MOV [W14-12], W3

	RCALL _sub_longs ;(out_max - out_min) 

	MOV [W14], W2
	MOV [W14+2], W3

	RCALL _mul_longs 

	MOV.D W0, [W14] ;(x - in_min) * (out_max - out_min) 

	MOV [W14-18], W0
	MOV [W14-16], W1
	MOV [W14-22], W2
	MOV [W14-20], W3

	RCALL _sub_longs

	MOV W0, W2
	MOV W1, W3

	MOV [W14], W0
	MOV [W14+2], W1

	RCALL _div_longs

	MOV [W14-14], W2
	MOV [W14-12], W3

	RCALL _add_longs

	POP W3
	POP W2
	
	ULNK
	
	RETURN