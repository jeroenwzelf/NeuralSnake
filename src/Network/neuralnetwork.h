#pragma once

#include "game.h"
#include "structures.h"
#include <memory>

typedef std::pair<std::vector<coordinate>, std::vector<coordinate>> gamestate;

/*
 * This class only generates random input for now, and
 * a neural network does still need to be implemented
*/
class neural_network {
	public:
		neural_network(std::shared_ptr<game> g) : Game(g) {}
		const unsigned char get_input();	// generates input for the game
	private:
		gamestate view_game();	// get gamestate (position of snake and food)
	private:
		std::shared_ptr<game> Game;
};