#include "GL/glew.h"
#include "GL/glut.h"
#include "renderer.h"

#define FULLSCREEN false

/* -- OpenGL callbacks -- */

renderer *glut_callback;
std::shared_ptr<game> game_callback;

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

/* -- -- */

renderer::renderer(int argc, char *argv[], std::shared_ptr<game> g) {
	glut_callback = this;
	game_callback = g;
	init_glut(argc, argv);
}

void renderer::init_glut(int argc, char *argv[]) {
    // for transparent objects with textures with an alpha channel
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glShadeModel(GL_SMOOTH);

    // glut callback functions
	glutReshapeFunc(reshapeCallback);
	glutDisplayFunc(displayCallback);
	glutTimerFunc(50, glTimerCallback, 0);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(inputCallback);
	glutKeyboardUpFunc(inputUpCallback);

	glScalef(0.1f, 0.1f, 1);

	if (FULLSCREEN) glutFullScreen();
	glutMainLoop();
}

void renderer::glTimer(int value) {
	static int delay = 0;
	++delay;
	if (game_callback->running && delay == 1)
		game_callback->update();
	else if (delay > 1) delay = 0;

	glutPostRedisplay();
	glutTimerFunc(50, glTimerCallback, 0);
}

void renderer::display() {
	glClear(GL_COLOR_BUFFER_BIT);
	game_callback->handle_input();

	
	/* -- grid -- */
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

    /* -- borders -- */
    glBegin(GL_QUADS);
		glVertex2i( -10,	 9	);
		glVertex2i( -10,	9+1 );
		glVertex2i(-10+20,	9+1 );
		glVertex2i(-10+20,	 9	);
	glEnd();
	glBegin(GL_QUADS);
		glVertex2i(  -10,	 -10  );
		glVertex2i(  -10,	-10+1 );
		glVertex2i(-10+20,	-10+1 );
		glVertex2i(-10+20,	 -10  );
	glEnd();
	glBegin(GL_QUADS);
		glVertex2i( -10,	 -9	  );
		glVertex2i( -10,	-9+18 );
		glVertex2i(-10+1,	-9+18 );
		glVertex2i(-10+1,	 -9	  );
	glEnd();
	glBegin(GL_QUADS);
		glVertex2i(  9, 	 -9	  );
		glVertex2i(  9, 	-9+18 );
		glVertex2i( 9+1,	-9+18 );
		glVertex2i( 9+1,	 -9	  );
	glEnd();

	game_callback->draw();
	if (!game_callback->running) game_callback->draw_lose();

	glFlush();
}

void renderer::reshape (const int w, const int h) {
	screen_width = w; screen_height = h;
	// set screen size for game
    glViewport(0, 0, (GLsizei)screen_width, (GLsizei)screen_height);
}