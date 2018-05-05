// Implementation of the v1 ease servos without classes
// in order to use it with the PIC24

typedef struct ServoEaser {

	int servo_num;
    int frameMillis;  // minimum update time between servo moves
    int delay_after_move;
    int startPos;   // where servo started its tween
    int currPos;    // current servo position, best of our knowledge

    int changePos;  

    int durMillis;    
    int tick;         // count of easing moves within move duration 
    int tickCount;    // number of frames between start & end pos
    unsigned long lastMillis; // time time we did something

    // EasingFunc easingFunc; // func that describes tween motion
    // ArrivedFunc arrivedFunc; // func to call when servo arrives at dest
    // BufferEmptiedFunc bufferemptiedFunc;
    //Research about function pointers in c

    // Circular buffer
    int moves[MOVES_BUFFER_LEN];
    int moves_dur[MOVES_BUFFER_LEN];

    int buffer_start, buffer_end;
    bool buffer_full, buffer_empty;

    // Status values
    bool running;  // is servo easer running?
    bool arrived;  // has servo arrived at its destination

} ServoEaser;

void servoeaser_update(ServoEaser* servoeaser, ServoInfo* servo_info) {

	if ( (millis() - servoeaser->lastMillis) < servoeaser->frameMillis + servoeaser->delay_after_move || !servoeaser->running ) return;

	servoeaser->lastMillis = millis();
	servoeaser->delay_after_move = 0;

	servoeaser->currPos = ServoEaser_easeInOutCubic(
							servoeaser->tick,
							servoeaser->startPos,
							servoeaser->changePos,
							servoeaser->tickCount 
						  );	


	if (!servoeaser->arrived) servoeaser->tick++;

	servo_info->duty_cycle = servoeaser->currPos;

	// if (servoeaser->servo_num == 0) Serial_println(servoeaser->currPos);

	if (servoeaser->tick == servoeaser->tickCount = 1) servoeaser_getNextPos(servoeaser);


}

void servoeaser_getNextPos(ServoEaser* servoeaser) {

	// if (servoeaser->arrivedFunc != NULL) servoeaser->arrivedFunc(servoeaser->servo_num);

	servoeaser->arrived = true;

	if (servoeaser->buffer_empty) {

        // if (servoeaser->bufferemptiedFunc != NULL) bufferemptiedFunc();
        servoeaser->running = false; //System will continue when added another move
        return;
    
    }

    servoeaser->startPos  = servoeaser->currPos; // current position becomes new start position

    servoeaser->changePos = servoeaser->moves[buffer_start] - servoeaser->startPos;
    servoeaser->durMillis = servoeaser->moves_dur[buffer_start];

	servoeaser_inc_moves_start_pointer(servoeaser);

    servoeaser->tickCount = servoeaser->durMillis / servoeaser->frameMillis;
    servoeaser->tick = 0;

    servoeaser->arrived = false;


}

bool servoeaser_inc_moves_end_pointer(ServoEaser* servoeaser) {

    servoeaser->buffer_end = (servoeaser->buffer_end + 1) % MOVES_BUFFER_LEN;
    buffer_empty = false;

    if (servoeaser->buffer_end == servoeaser->buffer_start) {

        servoeaser->buffer_full = true;
        return true;   

    }

    return false;

}

bool servoeaser_inc_moves_start_pointer(ServoEaser* servoeaser) {

    servoeaser->buffer_start = (servoeaser->buffer_start + 1) % MOVES_BUFFER_LEN;
    servoeaser->buffer_full = false;

    if (servoeaser->buffer_end == servoeaser->buffer_start) {
        servoeaser->buffer_empty = true;
        return true;
    }
    return false;   

}

void servoeaser_init_moves_buffer(ServoEaser* servoeaser) {

    servoeaser->buffer_start = 0;
    servoeaser->buffer_end = 0;
    servoeaser->buffer_empty = true;
    servoeaser->buffer_full = false;
  
}

// set up an easer with just a servo and a starting position
void servoeaser_init(ServoEaser* servoeaser, int servo_num_, int frameTime) {

    servoeaser->servo_num = servo_num_;

    servoeaser_init_moves_buffer(servoeaser);

    servoeaser->arrived = true;
    servoeaser->running = false;

    servoeaser->frameMillis = frameTime;
    servoeaser->delay_after_move = 0;

    // servoeaser->easingFunc = ServoEaser_easeInOutCubic;
    // servoeaser->arrivedFunc = NULL;
    // servoeaser->bufferemptiedFunc = NULL;
    
}

// reset easer to initial conditions, does not nuke easingFunc or arrivedFunc
void servoeaser_reset(ServoEaser* servoeaser) {

    servoeaser->currPos = servoinfo[servo_num].duty_cycle;
    servoeaser->startPos = currPos;  // get everyone in sync
    servoeaser->changePos = 0;       // might be overwritten below

    if( !servoeaser->buffer_empty ) {
        servoeaser->changePos = servoeaser->moves[buffer_start] - servoeaser->startPos ;
        servoeaser->durMillis = servoeaser->moves_dur[buffer_start];
    }

    servoeaser_inc_moves_start_pointer(servoeaser);

    servoeaser->tickCount = (servoeaser->durMillis / servoeaser->frameMillis);
    servoeaser->tick = 0;
    
}

void servoeaser_addMoves(ServoEaser* servoeaser, int inp_moves[], int durations[], int moves_len) {

    for (int i = 0; i < moves_len; i++) {

        if (!servoeaser->buffer_full) {

            servoeaser->moves[buffer_end] = inp_moves[i];
            servoeaser->moves_dur[buffer_end] = durations[i];

            if (servoeaser_inc_moves_end_pointer(servoeaser)) return;

        } 
    }

}

void servoeaser_addMovesAndPlay(ServoEaser* servoeaser, int inp_moves[], int durations[], int moves_len) {

	for (int i = 0; i < moves_len; i++) {

        if (!servoeaser->buffer_full) {

            servoeaser->moves[buffer_end] = inp_moves[i];
            servoeaser->moves_dur[buffer_end] = durations[i];

            if (servoeaser_inc_moves_end_pointer(servoeaser)) return;

        } 
    }

    if (!servoeaser->running && servoeaser->arrived) servoeaser_play(servoeaser);
    else if (!servoeaser->running && !servoeaser->arrived) servoeaser_proceed(servoeaser);
    
}

bool servoeaser_buffer_has_space(ServoEaser* servoeaser, int positions_to_add) {

    if (servoeaser->buffer_full) return false;

    for (int i = 0; i < positions_to_add; i++) {

        if ( (servoeaser->buffer_end + 1) % MOVES_BUFFER_LEN == servoeaser->buffer_start ) return false;

    }

    return true;

}

void servoeaser_play(ServoEaser* servoeaser) {

    servoeaser->running = true;
    servoeaser->arrived = false;
    servoeaser->reset(servoeaser);

}

void servoeaser_stop_ease(ServoEaser* servoeaser) {

    servoeaser->running = false;

}

void servoeaser_proceed(ServoEaser* servoeaser) {

    if (!servoeaser->arrived || !servoeaser->buffer_empty) servoeaser->running = true;

}

void servoeaser_proceed_delay(ServoEaser* servoeaser, int delayaftermove) {

    servoeaser->delay_after_move = delayaftermove;

    if (!servoeaser->arrived || !buffer_empty) servoeaser->running = true;

}

bool servoeaser_isRuning(ServoEaser* servoeaser) {

    return servoeaser->running;

}