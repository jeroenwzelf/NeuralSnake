#pragma once

#include "structures.h"

struct food {
	food(coordinate l, color c) : loc(l), col(c) {}
	coordinate loc;
	color col;
};