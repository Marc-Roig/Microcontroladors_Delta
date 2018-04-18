.include "p24fj128ga010.inc"

.global _mul_longs
.global _mul_long_int
.global _div_longs
.global _div_long_int
.global _add_longs
.global _add_long_int_ret_long
.global _add_long_int_ret_int
.global _ASR_long
.global _SL_long
.global _float_to_long

_mul_longs:  ;CHECKED

	;W0 a1	long a
    ;W1 a2 
    ;W2 b1	long b
    ;W3 b2

    PUSH W4
    PUSH W5
    PUSH W6
    PUSH W7

    MUL.SS W1, W2, W4
    MUL.SS W0, W3, W6
    ADD W4, W6, W4
    ADD W4, W0, W1
    MUL.UU W0, W2, W0

    POP W7
    POP W6
    POP W5
    POP W4

    RETURN

_div_longs: ;CHECKED

	;a / b
	;W0 a1	long a
    ;W1 a2 
    ;W2 b1	long b
    ;W3 b2
	RCALL ___divsi3

	RETURN
	;returned long at W0 and W1

_div_long_int:

    ;W0 a1 long a
    ;W1 a2
    ;W2 b  int b

    PUSH W3

    MOV #0, W3

    RCALL _div_longs

    POP W3

    RETURN

_mul_long_int: ;CHECKED

    ;W0 a1 long a
    ;W1 a2
    ;W2 b  int b

    PUSH W3

    MOV #0, W3

    RCALL _mul_longs

    POP W3

    RETURN
    
_add_longs: ;CHECKED

    ;W0 a1
    ;W1 a2
    ;W2 b1
    ;W3 b2

    ADD W0, W2, W0
    ADDC W1, W3, W1

    RETURN


_add_long_int_ret_long: ;CHECKED
    
    ;W0 a1
    ;W1 a2
    ;W2 b
    
    ADD W0, W2, W0
    ADDC W1, #0, W1
    RETURN
    
_add_long_int_ret_int: ;CHECKED
    
    ;W0 a1
    ;W1 a2
    ;W2 b
    
    ADD W0, W2, W0
    RETURN

_ASR_long: ;CHECKED
	
	;W0 a1 , long a
	;W1 a2
	;W2 number of shifts
	
	shift_right_again:
		DEC W2, W2
		BRA N, no_right_shifts_left
		ASR W1, W1
		RRC W0, W0 ;rotate right through carry to take into account if previous ASR left a 1
		BRA shift_right_again

	no_right_shifts_left:
	
	RETURN
	
_SL_long: ;CHECKED

	;W0 a1 , long a
	;W1 a2
	;W2 number of shifts

	shift_left_again:
		DEC W2, W2	;decrement number of shifts left
		BRA N, no_left_shifts_left ;Check if number of shifts left > 0
		ADD W0, W0, W0 ;multiply by 2
		ADDC W1, W1, W1 ;multiply by 2 adding carry of previous opperation
		BRA shift_left_again

	no_left_shifts_left:
		
	RETURN

_float_to_long: ;CHECKED

	;W0	a1 -float a
	;W1 a2

	RCALL ___fixsfsi

	RETURN

