#pragma once

#include "structures.h"
#include "snake.h"
#include "food.h"
#include "input_handler.h"
#include "SoundEngine.h"
#include <memory>

class game {
	public:
		game(bool DM);
		void new_game();
		void draw();
		void update(float time);
		void draw_lose();
		void get_input();
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
		/* -- game state -- */
		static const unsigned WORLD_SIZE = 18;
		std::shared_ptr<Snake> snake;
		std::vector<food> all_food;
		int points, seconds;
		/* -- drawing -- */
		bool music;
		color backgroundcolor;
		/* -- meta handlers -- */
		bool running;
		std::shared_ptr<input_handler> inputHandler;
		std::shared_ptr<sound_engine> soundEngine;
		/* -- flags -- */
		bool DISCO_MODE;
};