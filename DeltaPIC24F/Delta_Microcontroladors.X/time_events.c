#include "Config.h"

void init_T4_Interrupt() {

	T4CON = 0X8000;
	PR4 = 3999;
	TMR4 = 0;
	_T4IF = 0;
	_T4IE = 0;

}

void delay(int valor_retard) { //in ms

    long delay_cycles = (long)valor_retard * 1000 / 16;
    TMR4 = 0;
    while (TMR4 < delay_cycles);

}

void _ISR _T4Interrupt() {

	#ifdef INOUTS_H

		// ADC_update_values();

	#endif

}