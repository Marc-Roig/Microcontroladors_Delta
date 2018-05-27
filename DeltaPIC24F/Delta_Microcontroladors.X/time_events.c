#include "Config.h"

void init_T4_Interrupt() {

	T4CON = 0X8000; //Enable timer
	PR4 = 3999; //pulse every 1 ms
	TMR4 = 0;
	_T4IF = 0;
	_T4IE = 1; //Enable interrupt

}

void delay(long valor_retard) { //in ms

    int delay_cycles = (int)(valor_retard * 1000 / 256);
    TMR4 = 0;
    while (TMR4 < delay_cycles);

}

void _ISR _T4Interrupt() {

	#ifdef INOUTS_H

		// ADC_update_values();

	#endif

}

int millis() {

	// long countTMR = TMR4;
	// return (int)(countTMR * 16 / 1000);
	return (int)TMR4;
	 
}