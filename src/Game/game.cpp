#include "game.h"

#include "GL/glew.h"
#include "GL/glut.h"

game::game() {
	inputHandler = std::make_shared<input_handler>();
	soundEngine = std::make_shared<sound_engine>();
	new_game();
}

void game::new_game() {
	running = true;
	points = 0;
	seconds = 0;
	snake = std::make_shared<Snake>(WORLD_SIZE);
	generate_food();
}

void game::update(float time) {
	/* -- add every second to time -- */
	static float milliseconds_elapsed = 0;
	milliseconds_elapsed += time;
	if (milliseconds_elapsed > 500) {
		++seconds; milliseconds_elapsed = 0;
	}
	/* -- food generation -- */
	if (food.empty()) generate_food();
	else if (randomfloat(0, 200) < 1) generate_food();
	/* -- snake move -- */
	if (!snake->move()) lose();
	/* -- snake food eat -- */
	for (auto part : snake->body) {
		for (auto f = food.begin(); f != food.end();) {
			if (part.x == f->x && part.y == f->y) {
				points += 100;
				snake->eat();
				soundEngine->play_nom();
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

/* -- draw objects over world -- */
void game::draw() {
	/* -- draw food -- */
	for (auto f : food) draw_gridpixel(f);
	/* -- draw snake -- */
	for (auto part : snake->body)
		draw_gridpixel(part, snake->snake_color);
	/* -- draw counters -- */
    std::string secondcounter = "SECONDS: " + std::to_string(seconds);
	glColor3f(0, 0, 0);
    glRasterPos2f(-7, 9.2);
    for (char& c : secondcounter) {
    	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)c);
    }
	std::string pointcounter = "POINTS: " + std::to_string(points);
	glColor3f(0, 0, 0);
    glRasterPos2f(1, 9.2);
    for (char& c : pointcounter) {
    	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)c);
    }
}

/* -- draws messagebox indicating the game ended -- */
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

/* -- draws white pixel with coordinate C on the grid -- */
void game::draw_gridpixel(coordinate C) {
	draw_gridpixel(C, color(0.75f, 0.75f, 0.75f));
}
/* -- draws colored pixel with coordinate C on the grid -- */
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
	/* -- when game over -- */
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