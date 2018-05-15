#include "game.h"

#include "GL/glew.h"
#include "GL/glut.h"

game::game(bool DM) {
	inputHandler = std::make_shared<input_handler>();
	soundEngine = std::make_shared<sound_engine>();
	DISCO_MODE = DM;
	music = DM;
	if (music) soundEngine->play_music();
	backgroundcolor.set(0, 0, 0);
	new_game();
}

void game::new_game() {
	running = true;
	points = 0;
	seconds = 0;
	int w = WORLD_SIZE; snake = std::make_shared<Snake>(w);
	all_food.clear();
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
	if (all_food.empty()) generate_food();
	else if (randomfloat(0, 200) < 1) generate_food();
	/* -- snake move -- */
	if (!snake->move()) lose();
	/* -- snake food eat -- */
	for (auto part : snake->body) {
		for (auto f = all_food.begin(); f != all_food.end();) {
			if (part.x == f->loc.x && part.y == f->loc.y) {
				points += 100;
				snake->eat();
				soundEngine->play_nom();
				if (DISCO_MODE) backgroundcolor.set(f->col);
				all_food.erase(f);
			}
			else ++f;
		}
	}
}

void game::lose() {
	running = false;
}

void game::generate_food() {
	color c;
	if (DISCO_MODE) c.randomize();
	else c.set(0.75, 0.75, 0.75);
	all_food.push_back( food(coordinate(randomfloat(0, WORLD_SIZE), randomfloat(0, WORLD_SIZE)), c ));
}

/* -- draw scene -- */
void game::draw() {
	glClearColor(backgroundcolor.r, backgroundcolor.g, backgroundcolor.b, 1.0);
	color gridcolor, snakecolor;
	if (DISCO_MODE) {
		snakecolor.randomize();
		gridcolor.set(backgroundcolor);
		gridcolor.inverse();
	}
	else {
		gridcolor.set(0.75f, 0.75f, 0.75f);
		snakecolor.set(snake->snake_color);
	}

	/* -- draw world grid -- */
	float HALF_GRID_SIZE = 9;
	glBegin(GL_LINES);
    glColor3f(gridcolor.r, gridcolor.g, gridcolor.b);
    for(float i = -HALF_GRID_SIZE; i <= HALF_GRID_SIZE; ++i) {
        glVertex2f(i, -HALF_GRID_SIZE);
        glVertex2f(i, HALF_GRID_SIZE);

        glVertex2f(-HALF_GRID_SIZE,	i);
        glVertex2f(HALF_GRID_SIZE,	i);
    }
    glEnd();

    /* -- draw world grid borders -- */
    glBegin(GL_QUADS);	// TOP BORDER
		glVertex2i( -10,	 9	);
		glVertex2i( -10,	9+1 );
		glVertex2i(-10+20,	9+1 );
		glVertex2i(-10+20,	 9	);
	glEnd();
	glBegin(GL_QUADS);	// BOTTOM BORDER
		glVertex2i(  -10,	 -10  );
		glVertex2i(  -10,	-10+1 );
		glVertex2i(-10+20,	-10+1 );
		glVertex2i(-10+20,	 -10  );
	glEnd();
	glBegin(GL_QUADS);	// LEFT BORDER
		glVertex2i( -10,	 -9	  );
		glVertex2i( -10,	-9+18 );
		glVertex2i(-10+1,	-9+18 );
		glVertex2i(-10+1,	 -9	  );
	glEnd();
	glBegin(GL_QUADS);	// RIGHT BORDER
		glVertex2i(  9, 	 -9	  );
		glVertex2i(  9, 	-9+18 );
		glVertex2i( 9+1,	-9+18 );
		glVertex2i( 9+1,	 -9	  );
	glEnd();

	/* -- draw food -- */
	for (auto const &f : all_food) draw_gridpixel(f.loc, f.col);
	/* -- draw snake -- */
	for (auto const &part : snake->body) draw_gridpixel(part, snakecolor);
	/* -- draw counters -- */
    std::string secondcounter = "SECONDS: " + std::to_string(seconds);
	glColor3f(backgroundcolor.r, backgroundcolor.g, backgroundcolor.b);
    glRasterPos2f(-7, 9.2);
    for (char& c : secondcounter) {
    	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)c);
    }
	std::string pointcounter = "POINTS: " + std::to_string(points);
	glColor3f(backgroundcolor.r, backgroundcolor.g, backgroundcolor.b);
    glRasterPos2f(1, 9.2);
    for (char& c : pointcounter) {
    	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)c);
    }
}

/* -- draws messagebox indicating the game ended -- */
void game::draw_lose() {
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
	float x = C.x-(static_cast<float>(WORLD_SIZE) / 2); float y = C.y-(static_cast<float>(WORLD_SIZE) / 2);
	glColor3f(col.r, col.g, col.b);
	glBegin(GL_QUADS);
		glVertex2f(  x ,	 y	);
		glVertex2f(  x ,	y+1 );
		glVertex2f( x+1,	y+1 );
		glVertex2f( x+1,	 y	);
	glEnd();
}

void game::get_input() {
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
				case 'm': 	if (music) { soundEngine->stop_music(); music = false; }
							else { soundEngine->play_music(); music = true; }
							break;
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