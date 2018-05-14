#include "AI.h"

class AI_simple : public AI {
	public:
		AI_simple(std::shared_ptr<game> g) : AI(g) {}
		const unsigned char get_input() override;
};

inline const unsigned char AI_simple::get_input() {
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