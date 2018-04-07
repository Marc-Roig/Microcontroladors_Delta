typedef struct Sequence {

	float moves[4][SEQUENCE_MAX_MOVES]; //{ {dc1_1, dc2_1, dc3_1}, {dc1_2, dc2_2, dc3_2} ...}
	int last_move;

	bool moves_full;
	bool playing;
	bool arrived;

} Sequence;

Sequence sequence;

void init_sequence() {

	last_move = 0;

	sequence.moves_full = false;

	sequence.playing = false;
	sequence.arrived = false;

}

void sequence_update() {

	static char S0 = 0;

	if (playing) return;

    bool save_pos_button = digitalRead(SEQUENCE_SAVE_POS_BUTTON_PIN);
    
    if (save_pos_button && !S0) {
    
      new_move(seq);
      S0 = 1;
      
    } else if (!save_pos_button) S0 = 0;

}

void new_move() {

	if (sequence.moves_full) return;

	sequence.moves[0][last_move] = servoinfo[0].duty_cycle;
	sequence.moves[1][last_move] = servoinfo[1].duty_cycle;
	sequence.moves[2][last_move] = servoinfo[2].duty_cycle;
	sequence.moves[3][last_move] = servoinfo[3].duty_cycle;

	if (last_move == SEQUENCE_MAX_MOVES - 1) sequence.moves_full = true;
	else last_move++;

}

void play_sequence(bool servos_eased, ServoEaser servo_easers[4]) {

	//dump into ease servos
	int* durations[last_move+1]; 

	if (sequence.playing) return;

	for (int i = 0; i < last_move+1; i++) {
		durations[i] = 1000;
	}

	serv_easer[0].addMoves(sequence.moves[0], durations, last_move + 1);
	serv_easer[0].play();

	serv_easer[1].addMoves(sequence.moves[1], durations, last_move + 1);
	serv_easer[1].play();

	serv_easer[2].addMoves(sequence.moves[2], durations, last_move + 1);
	serv_easer[2].play();

	serv_easer[3].addMoves(sequence.moves[3], durations, last_move + 1);
	serv_easer[3].play();

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