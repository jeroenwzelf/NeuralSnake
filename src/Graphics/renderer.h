#pragma once

#include "game.h"

class renderer {
	public:
		renderer(int argc, char *argv[], std::shared_ptr<game> g);
		void display();
		void reshape(const int w, const int h);
		void glTimer(int value);
	private:
		void init_glut(int argc, char *argv[]);
	private:
		int screen_width, screen_height;
};