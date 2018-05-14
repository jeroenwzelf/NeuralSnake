#include "GL/glew.h"
#include "GL/glut.h"
#include "renderer.h"

#define FULLSCREEN false

/* -- global OpenGL callbacks -- */
/* -- (OpenGL doesn't like functions from references to classes) -- */

renderer *glut_callback;
std::shared_ptr<game> game_callback;
std::shared_ptr<AI> AI_callback;

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
void AIinputCallback(const unsigned char key) {
	game_callback->inputHandler->inputFunc(key);
}

/* -- -- */

/* -- set callbacks and init glut -- */
renderer::renderer(int argc, char *argv[],
				std::shared_ptr<game> g, std::shared_ptr<AI> n,
				bool USR_PLAY) {
	glut_callback = this;
	game_callback = g;
	AI_callback = n;
	USER_PLAY = USR_PLAY;
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
	/* -- keep real time in milliseconds -- */
	static int lastTime;
	int thisTime;
	float time;
	thisTime = glutGet(GLUT_ELAPSED_TIME);
	time = (thisTime - lastTime);
	lastTime = thisTime;

	/* -- update game state -- */
	static int delay = 0;
	++delay;
	if (game_callback->running && delay == 1)
		game_callback->update(time);
	else if (delay > 1) delay = 0;

	glutPostRedisplay();
	glutTimerFunc(50, glTimerCallback, 0);
}

/* -- actual scene rendering -- */
void renderer::display() {
	glClear(GL_COLOR_BUFFER_BIT);

	/* -- handle (user) input -- */
	if (!USER_PLAY) AIinputCallback(AI_callback->get_input());
	game_callback->get_input();
	
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