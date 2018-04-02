#pragma once

#include <random>

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