#include "AI.h"

namespace NN {
	static const int PERCEPTRON_WEIGHTS_AMOUNT = 10;
}

struct Perceptron {
	Perceptron();
	int feed_forward(float inputs[]);
	float weights[NN::PERCEPTRON_WEIGHTS_AMOUNT];
};

class AI_neural : public AI {
	public:
		AI_neural(std::shared_ptr<game> g) : AI(g) {}
		const unsigned char get_input() override;
};