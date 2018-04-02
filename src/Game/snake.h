#pragma once

#include <vector>
#include "structures.h"

class Snake {
	public:
		Snake(int W);
		bool move();
		void eat();

		void up();
		void down();
		void left();
		void right();
	public:
		std::vector<coordinate> body;
		color snake_color;
	private:
		int WORLD_MAX;
		enum direction { UP, DOWN, LEFT, RIGHT };
		direction snake_direction;
		bool turning;
		bool ate;
};