#include "neuralnetwork.h"

const unsigned char neural_network::get_input() {
	/* -- neural network generates completely random input -- */
	switch(randomint(0, 3)) {
		case 0: return 'w'; break;
		case 1: return 's'; break;
		case 2: return 'a'; break;
		case 3: return 'd'; break;
	}
	return 'w';
}

/* -- get gamestate for neural network to analyse -- */
gamestate neural_network::view_game() {
	gamestate S;
	S.first = Game->snake->body;
	S.second = Game->food;
	return S;
}