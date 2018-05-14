#include "AI_neural.h"

Perceptron::Perceptron() {
	for (int i=0; i < NN::PERCEPTRON_WEIGHTS_AMOUNT; ++i)
		weights[i] = randomfloat(-1, 1);
}

int Perceptron::feed_forward(float inputs[]) {
	float sum = 0;
	for (int i=0; i < NN::PERCEPTRON_WEIGHTS_AMOUNT; ++i) {
		sum += inputs[i] * weights[i];
	}
	return static_cast<int>(sum);
}

const unsigned char AI_neural::get_input() {
	return 27;
}