#include "game.h"

#include "GL/glew.h"
#include "GL/glut.h"

game::game(float screen_w, float screen_h) {
	running = true;
	inputHandler = std::make_shared<input_handler>();
	snake = std::make_shared<Snake>(WORLD_SIZE);
	generate_food();
}

void game::update(float time) {
	static int delay = 0;

	/* -- food generation -- */
	if (randomfloat(0, 100) < 1) generate_food();
	/* -- snake move -- */
	if (delay == 1) {
		if (!snake->move()) lose();
		delay = 0;
	}
	else ++delay;
	/* -- snake food eat -- */
	for (auto part : snake->body) {
		for (auto f = food.begin(); f != food.end();) {
			if (part.x == f->x && part.y == f->y) {
				snake->eat();
				food.erase(f);
			}
			else ++f;
		}
	}
}

void game::lose() {
	running = false;
}

void game::generate_food() {
	food.push_back( coordinate(randomfloat(0, WORLD_SIZE), randomfloat(0, WORLD_SIZE)) );
}

void game::draw() {
	draw_food();
	draw_snake();
}

void game::draw_gridpixel(coordinate C) {
	draw_gridpixel(C, color(0.75f, 0.75f, 0.75f));
}

void game::draw_gridpixel(coordinate C, color col) {
	float x = C.x-(WORLD_SIZE / 2); float y = C.y-(WORLD_SIZE / 2);
	glColor3f(col.r, col.g, col.b);
	glBegin(GL_QUADS);
		glVertex2f(  x ,	 y	);
		glVertex2f(  x ,	y+1 );
		glVertex2f( x+1,	y+1 );
		glVertex2f( x+1,	 y	);
	glEnd();
}

void game::draw_food() {
	for (auto f : food) {
		draw_gridpixel(f);
	}
}

void game::draw_snake() {
	for (auto part : snake->body) {
		draw_gridpixel(part, snake->snake_color);
	}
}

void game::handle_input() {
	if (running) {
		// keypress continuous actions (in keys_down)
		for (unsigned int i = 0; i < inputHandler->keys_down.size(); ++i) {
			unsigned char key = inputHandler->keys_down.at(i);
			switch(key) {
			}
		}
		// keypress event actions (in keys_pressed)
		for (unsigned int i = 0; i < inputHandler->keys_pressed.size(); ++i) {
			unsigned char key = inputHandler->keys_pressed.at(i);
			switch(key) {
				case 's': snake->down();	break;
				case 'a': snake->left();	break;
				case 'd': snake->right();	break;
				case 'q':
				case 27 :	// 'esc'
					exit(0); break;
			}
		}
	}
	else {
		// keypress continuous actions (in keys_down)
		for (unsigned int i = 0; i < inputHandler->keys_down.size(); ++i) {
			unsigned char key = inputHandler->keys_down.at(i);
			switch(key) {

			}
		}
		// keypress event actions (in keys_pressed)
		for (unsigned int i = 0; i < inputHandler->keys_pressed.size(); ++i) {
			unsigned char key = inputHandler->keys_pressed.at(i);
			switch(key) {
				case 'q':
				case 27 :	// 'esc'
					exit(0); break;
			}
		}
	}
	inputHandler->keys_pressed.clear();
	glutPostRedisplay();
}