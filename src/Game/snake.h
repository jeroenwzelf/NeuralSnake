#pragma once

#include <vector>
#include "structures.h"

class Snake {
	public:
		Snake(int W);
		bool move();
		void eat();
		/* -- change snake direction -- */
		void up();
		void down();
		void left();
		void right();
	public:
		std::vector<coordinate> body;
		color snake_color;
	private:
		int WORLD_MAX;	// grid limits
		enum direction { UP, DOWN, LEFT, RIGHT };
		direction snake_direction;
		bool turning;	// is snake turning this frame?
		bool ate;		// did snake eat food last frame?
};