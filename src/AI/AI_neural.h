#include "AI.h"
#include <cassert>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <iostream>

class Neuron;
typedef std::vector<Neuron> Layer;

struct Connection {
	Connection() { weight = randomfloat(0, 1); weight_difference = 0; }
	float weight;
	float weight_difference;

	std::ostringstream print() const {
		std::ostringstream ss;
		ss << std::setprecision(std::numeric_limits<float>::digits10+1);
		ss  << "#" << weight
			<< "#" << weight_difference;
		return ss;
	}
};

class Neuron {
	public:
		/* fill connections with random weights */
		Neuron(unsigned outputs, unsigned i);
		void feed_forward(const Layer &previous_layer);
		void calcGradients_output(unsigned target);
		void calcGradients_hidden(const Layer &next_layer);
		void update_weights(Layer &previous_layer);
		std::vector<Connection> output_weights;
		unsigned weight_index;
		float output;
		float gradient;
	private:
		static float activation_function(float sum) { return tanh(sum); }
		static float activation_function_derivative(float sum) { return 1.0 - sum * sum; }
		static float alpha;
		static float eta;
	public:
		std::ostringstream print() const {
			std::ostringstream ss;
			ss << std::setprecision(std::numeric_limits<float>::digits10+1);
			ss  << "#" << output
				<< "#" << gradient
				<< "#" << weight_index;
			for (auto &c : output_weights)
				ss << c.print().str();
			return ss;
		}
};

class Neural_Network {
	public:
		/* -- settings element e = amount of neurons for layer e in network -- */
		Neural_Network() {}
		Neural_Network(std::vector<unsigned> settings);

		void feed_forward(const std::vector<float> &inputs);
		void back_propagation(const std::vector<float> &targets);
		void back_propagation(const float &target, const unsigned &index);

		void get_results(std::vector<float> &results) const;
		
		Neuron* get_neuron(unsigned layer, unsigned i) { return &network[layer][i]; }
	private:
		std::vector<Layer> network;
		float overall_error;
	public:
		std::ostringstream print() const {
			std::ostringstream ss;
			for (auto &l : network) ss << "#" << l.size()-1;
			ss << '\n';
			for (auto &l : network) for (auto &n : l)
				ss << n.print().str() << '\n';
			return ss;
		}
};

class AI_neural : public AI {
	public:
		AI_neural(std::shared_ptr<game> g) : AI(g) {
			std::ifstream file(neural_network_file);
			if (file.fail()) NN = Neural_Network( {6*g->WORLD_SIZE*g->WORLD_SIZE, g->WORLD_SIZE*6, g->WORLD_SIZE*4, 4} );
			else NN = load_neural_network_file(file);
			file.close();
		}
		const unsigned char get_input() override;
	private:
		Neural_Network NN;
		unsigned highest_output();
		unsigned char previous_input;
		/* -- functions regarding Q-learning -- */
		void observe_and_learn_gamestate();
		int reward();

		/* -- neural network loading and saving -- */
		const std::string neural_network_file = "NN_data";
		Neural_Network load_neural_network_file(std::ifstream &file) const;
		void save_in_neural_network_file() const;
};