#include "neuralnetwork.h"

const unsigned char neural_network::get_input() {
	/* -- neural network generates completely random input -- */
	gamestate GS = view_game();
	switch(randomint(0, 3)) {
		case 0: return 'w'; break;
		case 1: return 's'; break;
		case 2: return 'a'; break;
		case 3: return 'd'; break;
	}
	return 27;
}

/* -- get gamestate for neural network to analyse -- */
gamestate neural_network::view_game() {
	std::vector<coordinate> food;
	for (auto f : Game->all_food) food.push_back(f.loc);
	return gamestate(Game->snake->body, food, Game->points, Game->seconds);
}