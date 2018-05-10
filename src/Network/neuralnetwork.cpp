#include "neuralnetwork.h"

neural_network::neural_network(std::shared_ptr<game> g, agent A) : active_agent(A), Game(g) {}

const unsigned char neural_network::get_input() {
	gamestate GS = view_game();
	switch(active_agent) {
		case SIMPLE: return simple_ai(GS); break;
		case RANDOM: return random_ai(GS); break;
	}
	return 27;
}

/* -- get gamestate for neural network to analyse -- */
gamestate neural_network::view_game() {
	std::vector<coordinate> food;
	for (auto f : Game->all_food) food.push_back(f.loc);
	return gamestate(Game->snake->body, food, Game->points, Game->seconds, Game->WORLD_SIZE);
}

/* -- finds a shortest path to nearest food, blind to it's own body --*/
unsigned char neural_network::simple_ai(gamestate GS) {
	coordinate head = GS.snake.front();

	/* find closest food */
	float closest_distance = 10000;
	coordinate closest_food = head;
	for (auto &f : GS.food) {
		float d = sqrt( pow((f.x - head.x), 2) + pow((f.y - head.y), 2) );
		if (d < closest_distance) {
			closest_distance = d;
			closest_food = f;
		}
	}

	/* find the shortest path to the food (considering a continuous world) */
	int flare_1 = head.y; int flare_2 = head.y;		// search up/down
	int target = closest_food.y;
	for (int i=0; i < 2; ++i) {
		if (flare_1 != target) {
			/* look for closest path by sending a flare to each direction */
			while (flare_1 != target && flare_2 != target) {
				++flare_1; flare_1 = flare_1 % GS.WORLD_MAX;	// move flare_1 up/right
				--flare_2; if (flare_2 < 0) flare_2 = GS.WORLD_MAX-1;	// move flare_2 down/left
			}
			switch(i) {
				case 0: {
					if (flare_1 == target) return 'w';
					if (flare_2 == target) return 's';
				} break;
				case 1: {
					if (flare_1 == target) return 'd';
					if (flare_2 == target) return 'a';
				} break;
				default: printf("weird\n"); break;
			}
		}
		flare_1 = head.x; flare_2 = head.x;			// search left/right
		target = closest_food.x;
	}
	return 'w';
}

/* -- totally random movement -- */
unsigned char neural_network::random_ai(gamestate GS) {
	switch(randomint(0, 3)) {
		case 0: return 'w'; break;
		case 1: return 's'; break;
		case 2: return 'a'; break;
		case 3: return 'd'; break;
	}
	return 27;
}