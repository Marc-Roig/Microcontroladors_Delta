.include "p24fj128ga010.inc"

.global _servo1_writeMicroseconds
.global _servo2_writeMicroseconds
.global _servo3_writeMicroseconds
.global _servo_writeMicroseconds
.global _disengage_servos
.global _attach_servos


_servo1_writeMicroseconds:

	;W0 int microsec
	MOV W0, OC1RS
	RETURN

_servo2_writeMicroseconds:

	;W0 int microsec
	MOV W0, OC2RS
	RETURN

_servo3_writeMicroseconds:

	;W0 int microsec
	MOV W0, OC3RS
	RETURN

_servo_writeMicroseconds:
	
	;W0 int microsec
	;W1 int servo_num

	SL W1, W1 ;Servo_num * 2 = num of lines to jump
	ADD Func_calls, W1, W1
	RCALL W1

	Func_calls:

	RCALL _servo1_writeMicroseconds ;microsec left at W0
	BRA End
	RCALL _servo2_writeMicroseconds ;microsec left at W0
	BRA End
	RCALL _servo3_writeMicroseconds ;microsec left at W0

	End:

	RETURN

_disengage_servos:
	
	PUSH W0 ;Save whatever was in W0

	MOV #0xFFF8, W0

	AND W0, OC1CON, OC1CON ;Output Compare 1 channel is disabled
	AND W0, OC2CON, OC2CON ;Output Compare 2 channel is disabled
	AND W0, OC3CON, OC3CON ;Output Compare 3 channel is disabled

	POP W0

	RETURN

_servo_attach:
	
	; W0 - unsigned int pin -> left for compatibility with arduino
	; W1 - unsigned int servo_num

	;Check if servo_num is valid
	CP W1, #3
	BRA GE, End

	;Branch to the servo_num config bits
	MUL.UU W1, 6, W1
	ADD Set_bits, W1, W1
	BRA W1

	Set_bits:
	;OUTPUT COMPARE 1
	MOV #9999, PR1
	MOV #750, OC1RS
	MOV #750, OC1R
	MOV #0x0006, OC1CON
	MOV #0x8010, T1CON
	BRA End

	;OUTPUT COMPARE 2
	MOV #9999, PR2
	MOV #750, OC2RS
	MOV #750, OC2R
	MOV #0x0006, OC2CON
	MOV #0x8010, T2CON
	BRA End

	;OUTPUT COMPARE 3
	MOV #9999, PR3
	MOV #750, OC3RS
	MOV #750, OC3R
	MOV #0x0006, OC3CON
	MOV #0x8010, T3CON

	End:

	RETURN


_attach_servos:

	;OUTPUT COMPARE 1
	MOV #9999, PR1
	MOV #750, OC1RS
	MOV #750, OC1R
	MOV #0x0006, OC1CON
	MOV #0x8010, T1CON

	;OUTPUT COMPARE 2
	MOV #9999, PR2
	MOV #750, OC2RS
	MOV #750, OC2R
	MOV #0x0006, OC2CON
	MOV #0x8010, T2CON

	;OUTPUT COMPARE 3
	MOV #9999, PR3
	MOV #750, OC3RS
	MOV #750, OC3R
	MOV #0x0006, OC3CON
	MOV #0x8010, T3CON

	RETURN

