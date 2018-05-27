
;INOUTS
.global _pinMode
.global _digitalRead
.global _digitalWrite
.global _init_analog_input
.global _analogRead
.global _map2

;SERVO MOVEMENT
.global _init_ServoInfo
.global _check_servo_change_direction
.global _servos_initial_positions
.global _init_servos
.global _set_servo_movement_with_dc

;SERVOS
.global _servo1_writeMicroseconds
.global _servo2_writeMicroseconds
.global _servo3_writeMicroseconds
.global _servo_writeMicroseconds
.global _disengage_servos
.global _servo_attach
.global _attach_servos

;LONG OPERATIONS
.global _mul_longs
.global _mul_long_int
.global _div_longs
.global _div_long_int
.global _add_longs
.global _add_long_int_ret_long
.global _add_long_int_ret_int
.global _sub_longs
.global _sub_long_int
.global _sub_int_long
.global _ASR_long
.global _SL_long
.global _float_to_long

;TIME EVENTS
.global _init_T4_Interrupt
.global _init_T4
.global _delay

;CALIBRATION
.global _servo_calibration
.global _calibration_start
.global _calibration_change_dc_potentiometer
.global _degug_calibration
.global _calibration_change_dc_mode

.equ OUTPUT, 0
.equ INPUT, 1
.equ ANALOG_INPUT, 2

.equ HIGH, 1
.equ LOW, 0

.equ IO_RB3, 0 ;Do not change, analogRead will stop working
.equ IO_RB8, 1 ;Do not change, analogRead will stop working
.equ IO_RB9, 2 ;Do not change, analogRead will stop working
.equ IO_RE8, 3
.equ IO_RE9, 4
.equ IO_RA14, 5
.equ IO_RA15, 6

.equ MS_BETWEEN_ADC_CONVERSION, 50
