#pragma once

#include "game.h"
#include "structures.h"
#include <memory>

/*
 * struct describing a unique state of the game using
 * snake position, food position, and time and point counter
*/
struct gamestate {
	gamestate() : points(0), time(0) {}
	gamestate(std::vector<coordinate> S, std::vector<coordinate> F, int P, int T)
		: snake(S), food(F), points(P), time(T) {}
	std::vector<coordinate> snake, food;
	int points, time;
};

/*
 * This class only generates random input for now, and
 * a neural network does still need to be implemented
*/
class neural_network {
	public:
		neural_network(std::shared_ptr<game> g) : Game(g) {}
		const unsigned char get_input();	// generates input for the game
	private:
		gamestate view_game();
	private:
		std::shared_ptr<game> Game;
};