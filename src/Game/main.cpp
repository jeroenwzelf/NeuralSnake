#include "GL/glew.h"
#include "GL/glut.h"
#include "renderer.h"

// using srand(time(NULL))
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// For some reason, LIACS computers give a segfault whenever linking -lGL
// For some reason, forcing the compiler to think that it needs pthreads
// so it actually links the -pthread library solves the problem
#if defined(__unix__)
#include <pthread.h>
void* simpleFunc(void*) { return NULL; }
void forcePTHreadLink() {
	pthread_t t1;
	pthread_create(&t1, NULL, &simpleFunc, NULL);
}
#endif

#define DEF_TITLE "Neural Snake"

void initWindow(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(DEF_TITLE);
	glewExperimental = true;
	glewInit();
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	initWindow(argc, argv);

	/* -- set flags -- */
	static const int flagcount = 2;
	bool USER_PLAY = false;
	bool DISCO_MODE = false;
	agent a = RANDOM;
	for (int i=1; i<1+flagcount; ++i) {
		if (argc > i) {
			if (strcmp(argv[i], "-u") == 0) USER_PLAY = true;
			if (strcmp(argv[i], "-d") == 0) DISCO_MODE = true;
			if (strcmp(argv[i], "-simple") == 0) a = SIMPLE;
			if (strcmp(argv[i], "-neural") == 0) a = NEURAL;
		}
	}

	std::shared_ptr<game> g = std::make_shared<game>(DISCO_MODE);
	std::shared_ptr<AI> n;
	switch(a) {
		case SIMPLE:	n = std::make_shared<AI_simple>(g); break;
		case NEURAL:	n = std::make_shared<AI_neural>(g); break;
		default:		n = std::make_shared<AI_random>(g);
	}

	renderer Renderer(argc, argv, g, n, USER_PLAY);
	return 0;
}