#include "neuralnetwork.h"

neural_network::neural_network(std::shared_ptr<game> g, agent A) : active_agent(A), gamestate(g) {}

const unsigned char neural_network::get_input() {
	switch(active_agent) {
		case SIMPLE: return simple_ai(); break;
		case RANDOM: return random_ai(); break;
	}
	return 27;
}

/* -- finds a shortest path to nearest food, blind to it's own body --*/
unsigned char neural_network::simple_ai() {
	coordinate head = gamestate->snake->body.front();

	/* find closest food */
	float closest_distance = 10000;
	coordinate closest_food = head;
	for (auto &food : gamestate->all_food) {
		coordinate f = food.loc;
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
				++flare_1; flare_1 = flare_1 % gamestate->WORLD_SIZE;	// move flare_1 up/right
				--flare_2; if (flare_2 < 0) flare_2 = gamestate->WORLD_SIZE-1;	// move flare_2 down/left
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
unsigned char neural_network::random_ai() {
	switch(randomint(0, 3)) {
		case 0: return 'w'; break;
		case 1: return 's'; break;
		case 2: return 'a'; break;
		case 3: return 'd'; break;
	}
	return 27;
}