#pragma once

#include <random>

/* -- generates a random int between and including a and b -- */
inline int randomint(int a, int b) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(a, b);
	return dis(gen);
}

/* -- generates a random float between and including a and b -- */
inline float randomfloat(float a, float b) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(a, b);
	return dis(gen);
}

struct coordinate {
	coordinate() {}
	coordinate(int X, int Y) : x(X), y(Y) {}
	int x, y;
};

struct color {
	color() {}
	color(float R, float G, float B) : r(R), g(G), b(B) {}
	float r, g, b;
};