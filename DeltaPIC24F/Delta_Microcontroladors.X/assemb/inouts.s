.include "p24fj128ga010.inc"
.global _pinMode


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
	BRA END

	RB3_PIN:

		CP W1, #1
		BRA NZ, RB3_OUTPUT
		BSET TRISB, #3	
		
		RB3_OUTPUT:
		CP W1, #0
		BRA NZ, RB3_ANALOG_INP
		BCLR TRISB, #3

		RB3_ANALOG_INP:
		CP W1, #2
		BRA NZ, END
		CALL _InitAnalogInput ;int pin_name remains in W0

	RB8_PIN:

		CP W1, #1
		BRA NZ, RB8_OUTPUT
		BSET TRISB, #8	
		
		RB8_OUTPUT:
		CP W1, #0
		BRA NZ, RB8_ANALOG_INP
		BCLR TRISB, #8

		RB8_ANALOG_INP:
		CP W1, #2
		BRA NZ, END
		CALL _InitAnalogInput ;int pin_name remains in W0
	
	RB9_PIN:

		CP W1, #1
		BRA NZ, RB9_OUTPUT
		BSET TRISB, #9	
		
		RB9_OUTPUT:
		CP W1, #0
		BRA NZ, RB9_ANALOG_INP
		BCLR TRISB, #9

		RB9_ANALOG_INP:
		CP W1, #2
		BRA NZ, END
		CALL _InitAnalogInput ;int pin_name remains in W0
	
	RE8_PIN:

		CP W1, #1
		BRA NZ, RE8_OUTPUT
		BSET TRISE, #8	
		
		RE8_OUTPUT:
		CP W1, #0
		BRA NZ, END
		BCLR TRISE, #8

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
		
		RA14_OUTPUT:
		CP W1, #0
		BRA NZ, END
		BCLR TRISA, #14

	RA15_PIN:

		CP W1, #1
		BRA NZ, RA15_OUTPUT
		BSET TRISA, #15	
		
		RA15_OUTPUT:
		CP W1, #0
		BRA NZ, END
		BCLR TRISA, #15

	END:

	RETURN
