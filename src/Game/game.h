#pragma once

#include "structures.h"
#include "snake.h"
#include "input_handler.h"
#include <memory>

#define WORLD_SIZE 18

class game {
	public:
		game(float screen_w, float screen_h);
		void draw();
		void update(float time);
		void handle_input();
	private:
		/* -- events -- */
		void generate_food();
		void lose();

		/* -- drawing -- */
		void draw_gridpixel(coordinate C);
		void draw_gridpixel(coordinate C, color col);
		void draw_snake();
		void draw_food();
	public:
		bool running;
		std::shared_ptr<input_handler> inputHandler;
	private:
		std::shared_ptr<Snake> snake;
		std::vector<coordinate> food;
};