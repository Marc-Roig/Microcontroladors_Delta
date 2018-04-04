typedef struct Sequence {

	float moves[SEQUENCE_MAX_MOVES][]; //{ {dc1_1, dc2_1, dc3_1}, {dc1_2, dc2_2, dc3_2} ...}
	bool playing;
	bool arrived;

} Sequence;

Sequence sequence;

void init_sequence() {


}

void new_sequence() {



}

void play_sequence(bool servos_eased) {
	//dump into ease servos
}