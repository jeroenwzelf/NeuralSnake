#include "AI.h"

class AI_random : public AI {
	public:
		AI_random(std::shared_ptr<game> g) : AI(g) {}
		const unsigned char get_input() override;
};

inline const unsigned char AI_random::get_input() {
	switch(randomint(0, 3)) {
		case 0: return 'w'; break;
		case 1: return 's'; break;
		case 2: return 'a'; break;
		case 3: return 'd'; break;
	}
	return 27;
}