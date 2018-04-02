#include "game.h"

#include "GL/glew.h"
#include "GL/glut.h"

game::game() {
	inputHandler = std::make_shared<input_handler>();
	new_game();
}

void game::new_game() {
	running = true;
	snake = std::make_shared<Snake>(WORLD_SIZE);
	generate_food();
}

void game::update() {
	/* -- food generation -- */
	if (food.empty()) generate_food();
	else if (randomfloat(0, 200) < 1) generate_food();
	/* -- snake move -- */
	if (!snake->move()) lose();
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

void game::draw_lose() {
	draw();
	/* -- draw textbox -- */
	float x = -7.5; float y = -1.5;
	float w = 15; float h = 3;
	glColor3f(0.75, 0.75, 0.75);
	glBegin(GL_QUADS);
		glVertex2f(  x ,	 y	);
		glVertex2f(  x ,	y+h );
		glVertex2f( x+w,	y+h );
		glVertex2f( x+w,	 y	);
	glEnd();
	/* -- draw text -- */
	std::string text = "You just played yourself!";
	glColor3f(0, 0, 0);
    glRasterPos2f(-6.5, 0);
    for (char& c : text) {
    	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)c);
    }
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
		/* -- keypress continuous actions (in keys_down) -- */
		for (unsigned int i = 0; i < inputHandler->keys_down.size(); ++i) {
			unsigned char key = inputHandler->keys_down.at(i);
			switch(key) {

			}
		}
		/* -- keypress event actions (in keys_pressed) -- */
		for (unsigned int i = 0; i < inputHandler->keys_pressed.size(); ++i) {
			unsigned char key = inputHandler->keys_pressed.at(i);
			switch(key) {
				case 'w': snake->up();		break;
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
		/* -- keypress continuous actions (in keys_down) -- */
		for (unsigned int i = 0; i < inputHandler->keys_down.size(); ++i) {
			unsigned char key = inputHandler->keys_down.at(i);
			switch(key) {

			}
		}
		/* -- keypress event actions (in keys_pressed) -- */
		for (unsigned int i = 0; i < inputHandler->keys_pressed.size(); ++i) {
			unsigned char key = inputHandler->keys_pressed.at(i);
			switch(key) {
				case 'r': new_game(); break;
				case 'q':
				case 27 :	// 'esc'
					exit(0); break;
			}
		}
	}
	inputHandler->keys_pressed.clear();
	glutPostRedisplay();
}