.include "p24fj128ga010.inc"

.global _servo1_writeMicroseconds
.global _servo2_writeMicroseconds
.global _servo3_writeMicroseconds
.global _servo_writeMicroseconds
.global _disengage_servos
.global _attach_servos


_servo1_writeMicroseconds: ;CHECKED

	;W0 int microsec
	MOV W0, OC1RS
	RETURN

_servo2_writeMicroseconds: ;CHECKED

	;W0 int microsec
	MOV W0, OC2RS
	RETURN

_servo3_writeMicroseconds: ;CHECKED

	;W0 int microsec
	MOV W0, OC3RS
	RETURN

_servo_writeMicroseconds: ;CHECKED
	
	;W0 int microsec
	;W1 int servo_num

	CP W1, #0
	BRA Z, Servo1_write
	CP W1, #1
	BRA Z, Servo2_write
	CP W1, #2
	BRA Z, Servo3_write
	BRA End_WriteM

	Servo1_write:
		RCALL _servo1_writeMicroseconds ;microsec left at W0
		BRA End_WriteM

	Servo2_write:
		RCALL _servo2_writeMicroseconds ;microsec left at W0
		BRA End_WriteM

	Servo3_write:
		RCALL _servo3_writeMicroseconds ;microsec left at W0

	End_WriteM:

	RETURN

_disengage_servos: ;CHECKED
	
	PUSH.S
	MOV #OC1CON, W0 ;@OC1CON
	MOV #OC2CON, W1 ;@OC2CON
	MOV #OC3CON, W2 ;@OC3CON
	
	MOV #0xFFF8, W3
	AND W3, [W0], [W0] ; OC1CON = 0XFFF8 & OC1CON
	AND W3, [W1], [W1] ; OC2CON = 0XFFF8 & OC2CON
	AND W3, [W2], [W2] ; OC3CON = 0XFFF8 & OC3CON

	POP.S
	RETURN

_servo_attach: ;CHECKED
	
	; W0 - unsigned int pin -> left for compatibility with arduino
	; W1 - unsigned int servo_num

	;Check if servo_num is valid
	CP W1, #0
	BRA Z,Set_bits_1

	CP W1, #1
	BRA Z,Set_bits_2

	CP W1, #2
	BRA Z,Set_bits_3

	BRA End_Attach

	Set_bits_1:
		;OUTPUT COMPARE 1
		MOV #9999, W1
		MOV W1, PR1

		MOV #750, W1
		MOV W1, OC1RS

		MOV #750, W1
		MOV W1, OC1R

		MOV #0x0006, W1
		MOV W1, OC1CON

		MOV #0x8010, W1
		MOV W1, T1CON

		BRA End_Attach

	Set_bits_2:
		;OUTPUT COMPARE 2
		MOV #9999, W1
		MOV W1, PR2

		MOV #750, W1
		MOV W1, OC2RS

		MOV #750, W1
		MOV W1, OC2R

		MOV #0x0006, W1
		MOV W1, OC2CON

		MOV #0x8010, W1
		MOV W1, T2CON

		BRA End_Attach

	Set_bits_3:
		;OUTPUT COMPARE 3
		MOV #9999, W1
		MOV W1, PR3

		MOV #750, W1
		MOV W1, OC3RS

		MOV #750, W1
		MOV W1, OC3R

		MOV #0x0006, W1
		MOV W1, OC3CON

		MOV #0x8010, W1
		MOV W1, T3CON

	End_Attach:

	RETURN


_attach_servos: ;CHECKED
	
	PUSH W1
	;OUTPUT COMPARE 1
	MOV #9999, W1
	MOV W1, PR1
	MOV #750, W1
	MOV W1, OC1RS
	MOV #750, W1
	MOV W1, OC1R
	MOV #0x0006, W1
	MOV W1, OC1CON
	MOV #0x8010, W1
	MOV W1, T1CON

	;OUTPUT COMPARE 2
	MOV #9999, W1
	MOV W1, PR2
	MOV #750, W1
	MOV W1, OC2RS
	MOV #750, W1
	MOV W1, OC2R
	MOV #0x0006, W1
	MOV W1, OC2CON
	MOV #0x8010, W1
	MOV W1, T2CON

	;OUTPUT COMPARE 3
	MOV #9999, W1
	MOV W1, PR3
	MOV #750, W1
	MOV W1, OC3RS
	MOV #750, W1
	MOV W1, OC3R
	MOV #0x0006, W1
	MOV W1, OC3CON
	MOV #0x8010, W1
	MOV W1, T3CON

	POP W1

	RETURN

