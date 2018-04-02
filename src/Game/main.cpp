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

	std::shared_ptr<game> g = std::make_shared<game>();
	renderer Renderer(argc, argv, g);
	return 0;
}