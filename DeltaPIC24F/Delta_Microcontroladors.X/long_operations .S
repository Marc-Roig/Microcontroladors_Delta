.include "p24fj128ga010.inc"
.include "assembly_routines.inc"


/*********************************************************************
* Function: _mul_longs:
*
* Overview: Multiply two longs
*
* PreCondition: none
*
* Input: long n1 - W0, W1
*        long n2 - W2, W3
*
* Output: long result - W0, W1
*
********************************************************************/

_mul_longs:  ;CHECKED

    LNK #0X08

    MOV W0, [W14]
    MOV W1, [W14+2]
    MOV W2, [W14+4]
    MOV W3, [W14+6]

    MOV [W14+2], W1
    MOV [W14+4], W0
    MUL.SS W1, W0, W2
    MOV [W14+6], W1
    MOV [W14], W0
    MUL.SS W1, W0, W0
    ADD W2, W0, W2
    MOV [W14], W1
    MOV [W14+4], W0
    MUL.UU W1, W0, W0
    ADD W2, W1, W1

    ULNK

    RETURN



/*********************************************************************
* Function: _div_longs:
*
* Overview: Divide two longs 
*
* PreCondition: none
*
* Input: long n1 - W0, W1
*        long n2 - W2, W3
*
* Output: long result - W0, W1 - (n1/n2)
*
********************************************************************/

_div_longs: ;CHECKED

	;a / b
	;W0 a1	long a
    ;W1 a2 
    ;W2 b1	long b
    ;W3 b2
	PUSH W4
    PUSH W5
    PUSH W6
    
    RCALL ___divsi3
    
    POP W6
    POP W5
    POP W4
    RETURN
	;returned long at W0 and W1



/*********************************************************************
* Function: _div_long_int:
*
* Overview: Divide long and int
*
* PreCondition: none
*
* Input: long n1 - W0, W1
*        int  n2 - W2
*
* Output: long result - W0, W1 - (n1/n2)
*
********************************************************************/

_div_long_int:

    PUSH W3

    MOV #0, W3 ; transform n2 of 1 word into 2 words.

    RCALL _div_longs

    POP W3

    RETURN



/*********************************************************************
* Function: _mul_long_int:
*
* Overview: mult long and int
*
* PreCondition: none
*
* Input: long n1 - W0, W1
*        int  n2 - W2
*
* Output: long result - W0, W1 - (n1*n2)
*
********************************************************************/

_mul_long_int: ;CHECKED

    ;W0 a1 long a
    ;W1 a2
    ;W2 b  int b

    PUSH W3

    MOV #0, W3

    RCALL _mul_longs

    POP W3

    RETURN



/*********************************************************************
* Function: _add_longs:
*
* Overview: Add two longs
*
* PreCondition: none
*
* Input: long n1 - W0, W1
*        long n2 - W2, W3
*
* Output: long result - W0, W1 - (n1+n2)
*
********************************************************************/
    
_add_longs: ;CHECKED

    ADD W0, W2, W0
    ADDC W1, W3, W1

    RETURN



/*********************************************************************
* Function: _add_long_int_ret_long:
*
* Overview: Add long and int (2 words, 1 word) and return a long (2 words)
*
* PreCondition: none
*
* Input: long n1 - W0, W1
*        int n2  - W2
*
* Output: long result - W0, W1 - (n1+n2)
*
********************************************************************/

_add_long_int_ret_long: ;CHECKED
    
    ;W0 a1
    ;W1 a2
    ;W2 b
    
    ADD W0, W2, W0
    ADDC W1, #0, W1
    RETURN
    


/*********************************************************************
* Function: _add_long_int_ret_int:
*
* Overview: Add long and int (2 words, 1 word) and return a int (1 word)
*
* PreCondition: none
*
* Input: long n1 - W0, W1
*        int n2  - W2
*
* Output: int result - W0 - (n1+n2) -> works with lower word of n1
*
********************************************************************/

_add_long_int_ret_int: ;CHECKED
    
    ;W0 a1
    ;W1 a2
    ;W2 b
    
    ADD W0, W2, W0
    RETURN



/*********************************************************************
* Function: _sub_longs:
*
* Overview: Substract two longs (2 words each)
*
* PreCondition: none
*
* Input: long n1 - W0, W1
*        int n2  - W2, W3
*
* Output: int result - W0 - (n1+n2) -> works with lower word of n1
*
********************************************************************/

_sub_longs:
    
    ;W0 a1
    ;W1 a2
    ;W2 b1
    ;W3 b2

    SUB W0, W2, W0
    SUBB W1, W3, W1

    RETURN



/*********************************************************************
* Function: _sub_long_int:
*
* Overview: Add long and int (2 words, 1 word) and return a long (2 words)
*
* PreCondition: none
*
* Input: long n1 - W0, W1
*        int n2  - W2
*
* Output: long result - W0, W1 - (n1-n2)
*
********************************************************************/

_sub_long_int:
    
    ;W0 a1
    ;W1 a2
    ;W2 b

    PUSH W3
    MOV #0, W3

    SUB W0, W2, W0
    SUBB W1, W3, W1    

    POP W3
    RETURN

/*********************************************************************
* Function: _ASR_long:
*
* Overview: Arithmetic shift right long
*
* PreCondition: none
*
* Input: long n1 - W0, W1
*
* Output: long result - W0, W1
*
********************************************************************/

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

