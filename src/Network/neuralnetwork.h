#pragma once

#include "game.h"
#include "structures.h"
#include <memory>
#include <math.h>

enum agent { SIMPLE, RANDOM };

/*
 * a neural network does still need to be implemented
*/
class neural_network {
	public:
		neural_network(std::shared_ptr<game> g, agent A);
		const unsigned char get_input();
	private:
		agent active_agent;

		/* different agents */
		unsigned char simple_ai();
		unsigned char random_ai();
	private:
		std::shared_ptr<game> gamestate;
};