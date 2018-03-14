
#ifndef MAIN_H  //INCLUDE GUARD

    #define MAIN_H

    #include "xc.h"
    #include <p24fj128ga010.h>

	
    //--SERVOS--//

    void disengage_servos();

    void engage_servos();

    void servo1_write(int angle);
    void servo2_write(int angle);
    void servo3_write(int angle);

    void servo1_write_duty_cycle(int duty_cycle);
    void servo2_write_duty_cycle(int duty_cycle);
    void servo3_write_duty_cycle(int duty_cycle);

    int scale_angle(int angle);


    //--MISC--//
    void polsadors(int* angle);
    void show_dutycycle_leds(int duty_cycle);



#endif //MAIN_H