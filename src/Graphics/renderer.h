#pragma once

#include "game.h"
#include "neuralnetwork.h"

#include <cstring>
#include <cstdio>

class renderer {
	public:
		renderer(int argc, char *argv[],
			std::shared_ptr<game> g, std::shared_ptr<neural_network> n,
			bool USR_PLAY);
		void display();
		void reshape(const int w, const int h);
		void glTimer(int value);
	private:
		void init_glut(int argc, char *argv[]);
	private:
		int screen_width, screen_height;
		bool USER_PLAY;
};