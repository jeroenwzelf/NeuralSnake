#pragma once

#include "game.h"
#include "structures.h"
#include <memory>
#include <math.h>

enum agent { SIMPLE, RANDOM };

/*
 * struct describing a unique state of the game using
 * snake position, food position, and time and point counter
*/
struct gamestate {
	gamestate() : points(0), time(0) {}
	gamestate(std::vector<coordinate> S, std::vector<coordinate> F, int P, int T, int WM)
		: snake(S), food(F), points(P), time(T), WORLD_MAX(WM) {}
	std::vector<coordinate> snake, food;
	int points, time;
	int WORLD_MAX;
};

/*
 * a neural network does still need to be implemented
*/
class neural_network {
	public:
		neural_network(std::shared_ptr<game> g, agent A);
		const unsigned char get_input();
	private:
		gamestate view_game();
		agent active_agent;

		/* different agents */
		unsigned char simple_ai(gamestate GS);
		unsigned char random_ai(gamestate GS);
	private:
		std::shared_ptr<game> Game;
};