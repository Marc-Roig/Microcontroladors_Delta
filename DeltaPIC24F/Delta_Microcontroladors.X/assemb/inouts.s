.include "p24fj128ga010.inc"

.global _pinMode
.global _digitalRead

.extern _RB8_Analog_Active
.extern _RB9_Analog_Active
.extern _RB3_Analog_Active


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

