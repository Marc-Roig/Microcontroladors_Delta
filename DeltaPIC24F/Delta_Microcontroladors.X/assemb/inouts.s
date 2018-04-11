.include "p24fj128ga010.inc"

.global _pinMode
.global _digitalRead

.extern _RB8_Analog_Active
.extern _RB9_Analog_Active
.extern _RB3_Analog_Active


_pinMode:
	; int pin_name at W0
	; int mode at W1

	PIN_SELECTION:

	CP W0, #0
	BRA Z, RB3_PIN

	CP W0, #1
	BRA Z, RB8_PIN

	CP W0, #2
	BRA Z, RB9_PIN

	CP W0, #3
	BRA Z, RE8_PIN

	CP W0, #4
	BRA Z, RE9_PIN 

	CP W0, #5
	BRA Z, RA14_PIN 

	CP W0, #6
	BRA Z, RA15_PIN

	; CP W0, #7
	BRA END ;not a valid number

	RB3_PIN:

		CP W1, #1
		BRA NZ, RB3_OUTPUT
		BSET TRISB, #3	
		BRA END
		
		RB3_OUTPUT:
		CP W1, #0
		BRA NZ, RB3_ANALOG_INP
		BCLR TRISB, #3
		BRA END

		RB3_ANALOG_INP:
		CP W1, #2
		BRA NZ, END
		BSET _RB3_Analog_Active, 0
		BRA END

	RB8_PIN:

		CP W1, #1
		BRA NZ, RB8_OUTPUT
		BSET TRISB, #8	
		BRA END
		
		RB8_OUTPUT:
		CP W1, #0
		BRA NZ, RB8_ANALOG_INP
		BCLR TRISB, #8
		BRA END

		RB8_ANALOG_INP:
		CP W1, #2
		BRA NZ, END
		BSET _RB8_Analog_Active, 0
		BRA END
	
	RB9_PIN:

		CP W1, #1
		BRA NZ, RB9_OUTPUT
		BSET TRISB, #9
		BRA END	
		
		RB9_OUTPUT:
		CP W1, #0
		BRA NZ, RB9_ANALOG_INP
		BCLR TRISB, #9
		BRA END

		RB9_ANALOG_INP:
		CP W1, #2
		BRA NZ, END
		BSET _RB9_Analog_Active, 0
		BRA END
	
	RE8_PIN:

		CP W1, #1
		BRA NZ, RE8_OUTPUT
		BSET TRISE, #8	
		BRA END
		
		RE8_OUTPUT:
		CP W1, #0
		BRA NZ, END
		BCLR TRISE, #8
		BRA END

	RE9_PIN:

		CP W1, #1
		BRA NZ, RE9_OUTPUT
		BSET TRISE, #9	
		
		RE9_OUTPUT:
		CP W1, #0
		BRA NZ, END
		BCLR TRISE, #9

	RA14_PIN:

		CP W1, #1
		BRA NZ, RA14_OUTPUT
		BSET TRISA, #14	
		BRA END
		
		RA14_OUTPUT:
		CP W1, #0
		BRA NZ, END
		BCLR TRISA, #14
		BRA END

	RA15_PIN:

		CP W1, #1
		BRA NZ, RA15_OUTPUT
		BSET TRISA, #15	
		BRA END
		
		RA15_OUTPUT:
		CP W1, #0
		BRA NZ, END
		BCLR TRISA, #15
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
		BRA NZ, END:
		MOV _RA15, W0
		BRA END

	END:

	RETURN

