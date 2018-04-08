typedef struct Sequence {

	int moves[4][SEQUENCE_MAX_MOVES]; //{ {dc1_1, dc2_1, dc3_1}, {dc1_2, dc2_2, dc3_2} ...}
	int last_move;

	bool moves_full;
	bool playing;
	bool arrived;

} Sequence;

Sequence sequence;

void init_sequence() {

	sequence.last_move = 0;

	sequence.moves_full = false;

	sequence.playing = false;
	sequence.arrived = false;

}

void sequence_update() {

	static char S0 = 0;

	if (sequence.playing) return;

    bool save_pos_button = digitalRead(SEQUENCE_SAVE_POS_BUTTON_PIN);
    
    if (save_pos_button && !S0) {
    
      new_move();
      S0 = 1;
      
    } else if (!save_pos_button) S0 = 0;

}

void new_move() {

	if (sequence.moves_full) return;

	sequence.moves[0][sequence.last_move] = servoinfo[0].duty_cycle;
	sequence.moves[1][sequence.last_move] = servoinfo[1].duty_cycle;
	sequence.moves[2][sequence.last_move] = servoinfo[2].duty_cycle;
	sequence.moves[3][sequence.last_move] = servoinfo[3].duty_cycle;

	if (sequence.last_move == SEQUENCE_MAX_MOVES - 1) sequence.moves_full = true;
	else sequence.last_move++;

}

void play_sequence(ServoEaser servo_easers[4]) {

	//dump into ease servos
	int durations[sequence.last_move+1]; 

	if (sequence.playing) return;

	for (int i = 0; i < sequence.last_move+1; i++) {
		durations[i] = 1000;
	}

	servo_easers[0].addMoves(sequence.moves[0], durations, sequence.last_move + 1);
	servo_easers[0].play();

	servo_easers[1].addMoves(sequence.moves[1], durations, sequence.last_move + 1);
	servo_easers[1].play();

	servo_easers[2].addMoves(sequence.moves[2], durations, sequence.last_move + 1);
	servo_easers[2].play();

	servo_easers[3].addMoves(sequence.moves[3], durations, sequence.last_move + 1);
	servo_easers[3].play();

	sequence.playing = true;

}

void reset_sequence() {

	sequence.moves_full = false;
	sequence.last_move = 0;

}

void sequence_remove_last_move() {

	if (sequence.last_move > 0) sequence.last_move--;
}

void sequence_finalized() {

	//Choose if it has to repeat the movement or create a new one
	sequence.playing = false;
	reset_sequence();

}