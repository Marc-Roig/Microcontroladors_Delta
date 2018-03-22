#ifndef MAIN_H
    
    #include "xc.h"
    #include <p24fj128ga010.h>
	#include <stdbool.h>

	#include "defines.h"
	
	#include "serialcom.h"
	#include "servos.h"

    
	#define MAIN_H //Include guard

    //--MISC--//
    void polsadors(int* angle);
    void show_dutycycle_leds(int duty_cycle);
    void delay(int cicles);


#endif //MAIN_H