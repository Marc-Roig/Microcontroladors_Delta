// #include "Config.h"

// Sequence sequence;

// void init_sequence() {

// 	pinMode(SEQUENCE_SAVE_POS_BUTTON_PIN, INPUT);
// 	pinMode(SEQUENCE_PLAY_BUTTON_PIN, INPUT);

// 	sequence.last_move = 0;

// 	sequence.moves_full = false;

// 	sequence.playing = false;
// 	sequence.arrived = false;

// 	// ServoEaser servo_easers[4];

// }

// void sequence_update() {

// 	static char S0 = 0;

// 	if (sequence.playing) return;

//     bool save_pos_button = digitalRead(SEQUENCE_SAVE_POS_BUTTON_PIN);
//     bool play_sequence_button = digitalRead(SEQUENCE_PLAY_BUTTON_PIN);
    
//     if (save_pos_button && !S0) {
    
//       new_move();
//       S0 = 1;
      
//     } else if (!save_pos_button) S0 = 0;

//     if (play_sequence_button) play_sequence();


// }

// void new_move() {

// 	if (sequence.moves_full) return;
// 	Serial.write("ADDED NEW MOVE\n");


// 	sequence.moves[0][sequence.last_move] = servoinfo[0].duty_cycle;
// 	sequence.moves[1][sequence.last_move] = servoinfo[1].duty_cycle;
// 	sequence.moves[2][sequence.last_move] = servoinfo[2].duty_cycle;
// 	sequence.moves[3][sequence.last_move] = servoinfo[3].duty_cycle;

// 	if (sequence.last_move == SEQUENCE_MAX_MOVES - 1) sequence.moves_full = true;
// 	else sequence.last_move++;

// }

// void play_sequence() {

// 	//dump into ease servos
// 	int durations[sequence.last_move+1]; 

// 	if (sequence.playing) return;

// 	for (int i = 0; i < sequence.last_move+1; i++) {
// 		durations[i] = SEQUENCE_DEFAULT_MOVEMENT_TIME;
// 	}

// 	addMovesAndPlay(&servoseased[0], sequence.moves[0], durations, sequence.last_move + 1);
// 	addMovesAndPlay(&servoseased[1], sequence.moves[1], durations, sequence.last_move + 1);
// 	addMovesAndPlay(&servoseased[2], sequence.moves[2], durations, sequence.last_move + 1);
// 	addMovesAndPlay(&servoseased[3], sequence.moves[3], durations, sequence.last_move + 1);
	
// 	sequence.playing = true;

// }

// void reset_sequence() {

// 	sequence.moves_full = false;
// 	sequence.last_move = 0;

// }

// void sequence_remove_last_move() {

// 	if (sequence.last_move > 0) sequence.last_move--;
// }

// void sequence_finalized() {

// 	//Choose if it has to repeat the movement or create a new one
// 	sequence.playing = false;
// 	reset_sequence();

// }


// void sequence_confirm_next_move(int servo_num) {

// 	static bool finished_servos[] = {false, false, false, false};

// 	finished_servos[servo_num - 1] = true;

// 	stop_ease(&servoseased[servo_num]);

// 	if (finished_servos[0] && finished_servos[1] && finished_servos[2] && finished_servos[3]) {

// 		for (int i = 0; i < 4; ++i) {

// 			finished_servos[i] = false;
// 			proceed(&servoseased[i], 1000);

// 		}
		
// 	}

// }
