void init_T4_Interrupt() {

	T4CON = 0X8000;
	PR4 = 3999;
	TMR4 = 0;
	_T4IF = 0;
	_T4IE = 0;

}



void _ISR _T4Interrupt() {

	#ifdef INOUTS_H

		// ADC_update_values();

	#endif
}