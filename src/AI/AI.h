#pragma once

#include "game.h"
#include "structures.h"

class AI {
	public:
		AI(std::shared_ptr<game> g) : gamestate(g) {}
		virtual const unsigned char get_input() = 0;
	protected:
		std::shared_ptr<game> gamestate;
};