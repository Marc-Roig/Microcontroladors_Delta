#ifndef SEQUENCE_H
	
	#define SEQUENCE_H
	
	typedef struct Sequence {

		int moves[4][SEQUENCE_MAX_MOVES]; //{ {dc1_1, dc2_1, dc3_1}, {dc1_2, dc2_2, dc3_2} ...}
		int last_move;

		bool moves_full;
		bool playing;
		bool arrived;

	} Sequence;

#endif