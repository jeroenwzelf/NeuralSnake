#include "GL/glew.h"
#include "GL/glut.h"
#include "renderer.h"

#define FULLSCREEN false

/* -- global OpenGL callbacks -- */
/* -- (OpenGL doesn't like functions from references to classes) -- */

renderer *glut_callback;
std::shared_ptr<game> game_callback;
std::shared_ptr<neural_network> neural_network_callback;

void reshapeCallback(const int w, const int h) {
	glut_callback->reshape(w, h);
}
void displayCallback() {
	glut_callback->display();
}
void glTimerCallback(int value) {
	glut_callback->glTimer(value);
}
void inputCallback(const unsigned char key, const int x, const int y) {
	game_callback->inputHandler->keyboardFunc(key, x, y);
}
void inputUpCallback(const unsigned char key, const int x, const int y) {
	game_callback->inputHandler->keyboardUpFunc(key, x, y);
}
void neuralinputCallback(const unsigned char key) {
	game_callback->inputHandler->inputFunc(key);
}

/* -- -- */

/* -- set callbacks and init glut -- */
renderer::renderer(int argc, char *argv[], std::shared_ptr<game> g, std::shared_ptr<neural_network> n) {
	glut_callback = this;
	game_callback = g;
	neural_network_callback = n;
	USER_PLAY = false;
	init_glut(argc, argv);
}

/* -- init glut renderer -- */
void renderer::init_glut(int argc, char *argv[]) {
    // for transparent objects with textures with an alpha channel
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glShadeModel(GL_SMOOTH);

    // glut callback functions
	glutReshapeFunc(reshapeCallback);
	glutDisplayFunc(displayCallback);
	glutTimerFunc(50, glTimerCallback, 0);

	/* -- user can play the game if you call the program with './neuralsnake -u' -- */
	if (argc == 2 && strcmp(argv[1], "-u") == 0) USER_PLAY = true;

	// keyboard input functions
	if (USER_PLAY) {
		glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
		glutKeyboardFunc(inputCallback);
		glutKeyboardUpFunc(inputUpCallback);
	}

	// zoom camera out a bit
	glScalef(0.1f, 0.1f, 1);

	if (FULLSCREEN) glutFullScreen();
	glutMainLoop();
}

/* -- constant update -- */
void renderer::glTimer(int value) {
	static int delay = 0;
	++delay;
	if (game_callback->running && delay == 1)
		game_callback->update();
	else if (delay > 1) delay = 0;

	glutPostRedisplay();
	glutTimerFunc(50, glTimerCallback, 0);
}

/* -- actual scene rendering -- */
void renderer::display() {
	glClear(GL_COLOR_BUFFER_BIT);

	/* -- handle (user) input -- */
	if (!USER_PLAY) neuralinputCallback(neural_network_callback->get_input());
	game_callback->handle_input();
	
	/* -- draw world grid -- */
	float HALF_GRID_SIZE = 9;
	glBegin(GL_LINES);
    glColor3f(0.75f, 0.75f, 0.75f);
    for(float i = -HALF_GRID_SIZE; i <= HALF_GRID_SIZE; ++i) {
        glVertex2f(i, -HALF_GRID_SIZE);
        glVertex2f(i, HALF_GRID_SIZE);

        glVertex2f(-HALF_GRID_SIZE,	i);
        glVertex2f(HALF_GRID_SIZE,	i);
    }
    glEnd();

    /* -- draw grid borders -- */
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

	/* -- draw game objects -- */
	game_callback->draw();
	if (!game_callback->running) game_callback->draw_lose();

	glFlush();
}

/* -- reshapes rendering window accordingly -- */
void renderer::reshape (const int w, const int h) {
	screen_width = w; screen_height = h;
    glViewport(0, 0, (GLsizei)screen_width, (GLsizei)screen_height);
}