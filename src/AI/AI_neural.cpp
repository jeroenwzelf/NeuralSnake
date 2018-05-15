#include "AI_neural.h"

/* -- Momentum (alpha) :
	0.0: no momentum
	0.5: moderate momentum -- */
float Neuron::alpha = 0.5;
/* -- Learning rate (eta) :
	0.0: slow learner
	0.2: medium learner
	1.0: reckless learner -- */
float Neuron::eta = 0.15;

Neuron::Neuron(unsigned outputs, unsigned i) {
	weight_index = i;
	for (unsigned i = 0; i < outputs; ++i)
		output_weights.push_back(Connection());
	output = 0.0f; gradient = 0.0f;
}

void Neuron::feed_forward(const Layer &previous_layer) {
	float sum = 0.f;
	/* -- output = every previous_layer.output * specific weight bias nodes -- */
	for (auto &previous_neuron : previous_layer) {
		sum += previous_neuron.output * previous_neuron.output_weights[weight_index].weight;
	}
	output = Neuron::activation_function(sum);
}

void Neuron::calcGradients_output(unsigned target) {
	float difference = target - output;
	gradient = difference * Neuron::activation_function_derivative(output);
}

void Neuron::calcGradients_hidden(const Layer &next_layer) {
	float sum = 0.f;
	for (unsigned n = 0; n < next_layer.size(); ++n) {
		sum += output_weights[n].weight * next_layer[n].gradient;
	}
	gradient = sum * Neuron::activation_function_derivative(output);
}

void Neuron::update_weights(Layer &previous_layer) {
	/* -- the weights to be updated are the output_weights of previous_layer (backpropagation) -- */
	for (auto &neuron : previous_layer) {
		// the weight from the connection of neuron to *this
		float old_connection_weight = neuron.output_weights[weight_index].weight;
		float new_connection_weight = 
					// individual input magnified by gradient and train rate
					Neuron::eta * neuron.output * gradient
					// momentum (a fraction of the previous weight difference)
					* Neuron::alpha * old_connection_weight;
		neuron.output_weights[weight_index].weight_difference = new_connection_weight;
		neuron.output_weights[weight_index].weight += new_connection_weight;
	}
}

Neural_Network::Neural_Network(std::vector<unsigned> settings) {
	for (auto layer_size : settings) {
		unsigned layer_outputs;
		if (layer_size == settings.back()) layer_outputs = 0;
		else layer_outputs = layer_size;
		Layer L;
		for (unsigned i = 0; i <= layer_size; ++i) 
			L.push_back(Neuron(layer_outputs, i));
		network.push_back(L);
	}
	network.back().back().output = 1.0f;
}

void Neural_Network::feed_forward(const std::vector<float> &inputs) {
	assert(inputs.size() == network[0].size() - 1);
	/* -- first, set the input layer -- */
	for (unsigned i = 0; i < inputs.size(); ++i) {
		network[0][i].output = inputs[i];
	}

	/* -- propagate it through every hidden layer -- */
	Layer previous_layer = network.front();
	for (auto current_layer : network) {
		for (auto &neuron : current_layer) {
			neuron.feed_forward(previous_layer);
		}
		previous_layer = current_layer;
	}
}

void Neural_Network::back_propagation(const std::vector<float> &targets) {
	Layer &output_layer = network.back();

	/* -- Calculate overall error (in RMS) -- */
	overall_error = 0.f;
	for (unsigned n = 0; n < output_layer.size() - 1; ++n) {
		float difference = targets[n] - output_layer[n].output;
		overall_error += difference * difference;
	}
	overall_error /= output_layer.size();
	overall_error = sqrt(overall_error);

	/* -- Calculate output layer gradients -- */
	for (unsigned n = 0; n < output_layer.size() - 1; ++n) {
		output_layer[n].calcGradients_output(targets[n]);
	}

	/* -- Calculate gradients on hidden layers -- */
	for (unsigned layer = network.size() - 2; layer > 0; --layer) {
		for (auto &neuron : network[layer]) {
			neuron.calcGradients_hidden(network[layer+1]);
		}
	}

	/* -- Update all connection weights -- */
	for (unsigned layer = network.size() - 1; layer > 0; --layer) {
		for (auto &neuron : network[layer]) {
			neuron.update_weights(network[layer-1]);
		}
	}
}

void Neural_Network::back_propagation(const float &target, const unsigned &index) {
	Layer &output_layer = network.back();

	/* -- Calculate overall error (in RMS) -- */
	overall_error = 0.f;
	for (unsigned n = 0; n < output_layer.size() - 1; ++n) {
		float difference = 0;
		if (n == index) difference = target - output_layer[index].output;
		overall_error += difference * difference;
	}
	overall_error /= output_layer.size();
	overall_error = sqrt(overall_error);

	/* -- Calculate output layer gradients -- */
	output_layer[index].calcGradients_output(target);

	/* -- Calculate gradients on hidden layers -- */
	for (unsigned layer = network.size() - 2; layer > 0; --layer) {
		for (auto &neuron : network[layer]) {
			neuron.calcGradients_hidden(network[layer+1]);
		}
	}

	/* -- Update all connection weights -- */
	for (unsigned layer = network.size() - 1; layer > 0; --layer) {
		for (auto &neuron : network[layer]) {
			neuron.update_weights(network[layer-1]);
		}
	}
}

void Neural_Network::get_results(std::vector<float> &results) const {
	results.clear();
	for (auto &neuron : network.back()) results.push_back(neuron.output);
}

void AI_neural::load_neural_network_file(std::ifstream &file) {
	static const std::string delimiter = "#";
	std::vector<unsigned> settings;
	std::string line;

	/* -- get header containing the settings of Neural_Network -- */
	size_t pos = 0;
	std::getline(file, line);
	while ((pos = line.find(delimiter)) != std::string::npos) {
		if (pos != 0)
			settings.push_back( atoll((line.substr(0, pos).c_str())) );
		line.erase(0, pos + 1);
	}
	settings.push_back( atoll((line.substr(0, pos).c_str())) );

	//Neural_Network NN_file(settings);
	NN = Neural_Network(settings);
	/* -- read all available Neuron info and copy it into the Neural_Network -- */
	/* (this reads the rest of the file according to the header) */
	unsigned layer = 0;
	for (auto &neuron_amount : settings) {
		for (unsigned n = 0; n <= neuron_amount; ++n) {
			std::getline(file, line);

			/* -- parse neuron data from file -- */
			std::vector<float> neuron_data;
			size_t pos = 0;
			while ((pos = line.find(delimiter)) != std::string::npos) {
				if (pos != 0) 
					neuron_data.push_back( std::stof((line.substr(0, pos).c_str())) );
				line.erase(0, pos + 1);
			}
			neuron_data.push_back( std::stof((line.substr(0, pos).c_str())));

			/* -- put parsed data in corresponding neuron -- */
			//auto neuron = NN_file.get_neuron(layer, n);
			Neuron* neuron = &NN.network[layer][n];
			neuron->output = neuron_data[0];
			neuron->gradient = neuron_data[1];
			neuron->weight_index = neuron_data[2];
			if (neuron_data.size() > 3) {
				unsigned i = 3;
				for (auto &C : neuron->output_weights) {
					C.weight = neuron_data[i];
					C.weight_difference = neuron_data[i+1];
					i+=2;
				}
			}
		}
		++layer;
	}
}

void AI_neural::save_in_neural_network_file() const {
	std::ofstream file(neural_network_file);
	file << NN.print().str();
	file.close();
}

unsigned AI_neural::highest_output() {
	std::vector<float> results;
	NN.get_results(results);

	unsigned highest_result_index = 0;
	for (unsigned i=1; i < results.size()-1; ++i) {
		if (results[i] > results[highest_result_index])
			highest_result_index = i;
	}
	return highest_result_index;
}

void AI_neural::observe_and_learn_gamestate() {
	static const float gamma = 0.9f; // it takes a lot of moves to win, so gamma set to high
	/* -- set up input vector for Neural Network -- */
	/* (input contains 3 cell layers of the whole game grid:
			0: empty cell
			1: snake cell
			2: food cell 	) -- */
	typedef std::vector<float> cell;
	typedef std::vector<cell> GRID_rows;
	std::vector<GRID_rows> GRID;

	GRID_rows emptyrow;
	for (unsigned i=0; i < gamestate->WORLD_SIZE; ++i) GRID.push_back(emptyrow);
	for (auto &row : GRID) {
		for (unsigned i = 0; i < gamestate->WORLD_SIZE; ++i) {
			cell c = {1, 0, 0};
			row.push_back(c);
		}
	}
	for (auto const &part : gamestate->snake->body) {
		GRID[part.y][part.x][0] = 0;
		GRID[part.y][part.x][1] = 1;
	}
	for (auto const &food : gamestate->all_food) {
		GRID[food.loc.y][food.loc.x][0] = 0;
		GRID[food.loc.y][food.loc.x][2] = 1;
	}

	/* -- convert 3D vector to 1D vector -- */
	std::vector<float> inputs;
	for (auto const &row : GRID) for (auto const &cell : row) for (auto const &layer : cell)
		inputs.push_back(layer);

	NN.feed_forward(inputs);

	float target = reward() + (gamma * highest_output());
	unsigned input_index;
	switch (previous_input) {
		case 'w': input_index = 0; break;
		case 's': input_index = 1; break;
		case 'a': input_index = 2; break;
		case 'd': input_index = 3; break;

	}
	NN.back_propagation(target, input_index);
}

int AI_neural::reward() {
	return gamestate->points;
}

const unsigned char AI_neural::get_input() {
	if (!gamestate->running) {
		--epochs;
		save_in_neural_network_file();
		if (epochs > 0) return 'r';
		return 'q';
	}
	/* -- train the neural network -- */
	observe_and_learn_gamestate();

	static float epsilon = 1;
	/* -- set up input vector for Neural Network -- */
	/* (input contains 3 cell layers of the whole game grid:
			0: empty cell
			1: snake cell
			2: food cell 	) -- */
	typedef std::vector<float> cell;
	typedef std::vector<cell> GRID_rows;
	std::vector<GRID_rows> GRID;

	GRID_rows emptyrow;
	for (unsigned i=0; i < gamestate->WORLD_SIZE; ++i) GRID.push_back(emptyrow);
	for (auto &row : GRID) {
		for (unsigned i = 0; i < gamestate->WORLD_SIZE; ++i) {
			cell c = {1, 0, 0};
			row.push_back(c);
		}
	}
	for (auto const &part : gamestate->snake->body) {
		GRID[part.y][part.x][0] = 0;
		GRID[part.y][part.x][1] = 1;
	}
	for (auto const &food : gamestate->all_food) {
		GRID[food.loc.y][food.loc.x][0] = 0;
		GRID[food.loc.y][food.loc.x][2] = 1;
	}

	/* -- convert 3D vector to 1D vector -- */
	std::vector<float> inputs;
	for (auto const &row : GRID) for (auto const &cell : row) for (auto const &layer : cell)
		inputs.push_back(layer);

	NN.feed_forward(inputs);

	unsigned char INPUT = 27;
	/* -- choose between a random action or a neural network action -- */
	if (randomfloat(0, 1) < epsilon) {
		switch (randomint(0, 3)) {
			case 0: INPUT = 'w'; break;
			case 1: INPUT = 's'; break;
			case 2: INPUT = 'a'; break;
			case 3: INPUT = 'd'; break;
		}
	}
	else {
		switch (highest_output()) {
			case 0: INPUT = 'w'; break;
			case 1: INPUT = 's'; break;
			case 2: INPUT = 'a'; break;
			case 3: INPUT = 'd'; break;
			default: INPUT = 27;
		}
	}
	if (epsilon > 0.02) epsilon -= 0.01;
	if (INPUT == 27) save_in_neural_network_file();
	previous_input = INPUT;
	return INPUT;
}

