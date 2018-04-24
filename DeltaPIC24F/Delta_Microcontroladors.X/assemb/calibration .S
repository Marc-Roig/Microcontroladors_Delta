.include "p24fj128ga010.inc"
.include "assembly_macros.inc"

.data

	change_dc_mode: .int CHANGE_WITH_POTENTIOMETER

	S1_cdb: .int 0 ;switch 1 static variable for Change dc With Buttons
	S2_cdb: .int 0 ;switch 2 static variable for Change dc With Buttons
	S3_cdb: .int 0 ;switch 3 static variable for Change dc With Buttons

	step_val_cdb: .int 0 ;step change from pessing buttons for Change dc With Buttons

.text

.extern servoinfo


_servo_calibration:
	
	
	RETURN
	
_calibration_change_dc_potentiometer:
	
	;W0 int servo num

	PUSH W1
	PUSH W2
	PUSH W3
	PUSH W4
	PUSH W5


	get_servoinfo_mem_pos W0
	
	MOV [W0+servinf_min_dc], W3
	MOV [W0+servinf_max_dc], W4
	ADD #servinf_duty_cycle, W0, W5

	MOV #CALIBRATION_POTE_PIN, W0
	RCALL _analogRead

	MOV #0, W1
	MOV #1023, W2
	RCALL _map

	MOV W0, [W5]


	POP W5
	POP W4
	POP W3
	POP W2
	POP W1

	RETURN





.end
	
	


 

		