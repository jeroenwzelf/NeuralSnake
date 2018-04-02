#include "snake.h"

Snake::Snake(int W) : WORLD_MAX(W), snake_direction(UP) {
	turning = false;
	/* -- create snake body -- */
	body.push_back(coordinate(W/2, W/2));
	body.push_back(coordinate(W/2, (W/2)-1));
	body.push_back(coordinate(W/2, (W/2)-2));
	/* -- generate snake color -- */
	do {
		snake_color.r = randomfloat(0.0, 1.0);
		snake_color.g = randomfloat(0.0, 1.0);
		snake_color.b = randomfloat(0.0, 1.0);
	} while (snake_color.r < 0.4 && snake_color.g < 0.4 && snake_color.b < 0.4);
}

bool Snake::move() {
	/* -- determine snake direction -- */
	coordinate head = body.front();
	switch(snake_direction) {
		case UP:	++head.y; head.y = head.y % (WORLD_MAX); break;
		case DOWN:	--head.y; if (head.y < 0) head.y = WORLD_MAX-1; break;
		case LEFT:	--head.x; if (head.x < 0) head.x = WORLD_MAX-1; break;
		case RIGHT:	++head.x; head.x = head.x % (WORLD_MAX); break;
	}
	/* -- check if snake is colliding with itself -- */
	for (auto part : body) 
		if (part.x == head.x && part.y == head.y) return false;

	/* -- move snake -- */
	body.insert(body.begin(), head);
	if (!ate) body.pop_back();
	else ate = false;
	turning = false;

	return true;
}

/* -- snake commands -- */
void Snake::eat() 	{ ate = true; }
void Snake::up()	{ if (!turning && snake_direction != DOWN)	{ snake_direction = UP;	turning = true; } }
void Snake::down()	{ if (!turning && snake_direction != UP)	{ snake_direction = DOWN; turning = true; } }
void Snake::left()	{ if (!turning && snake_direction != RIGHT)	{ snake_direction = LEFT; turning = true; } }
void Snake::right()	{ if (!turning && snake_direction != LEFT)	{ snake_direction = RIGHT; turning = true; } }