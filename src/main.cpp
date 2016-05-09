#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cmath>
#include <vector>

#define MAX_ERROR 0.01
#define SPEED 1

struct Neuron {
	std::vector<float> weights; // Weights of connexions with next layer
};

struct Layer {
	std::vector<struct Neuron*> neurons;
};

struct Network {
	std::vector<struct Layer*> layers;
};

//----------------------------------------------------------------------
// Initialize network
// @params :
//		- network
//		- number of layers in the network
//		- number of neurons for each layer
//----------------------------------------------------------------------
void init_network(struct Network* n, int numLayers, int numNeurons[]) {
	struct Layer layers[numLayers];

	// Iterate on layers (except last layer)
	for (int i = 0 ; i < numLayers - 1 ; i++) {
		struct Layer* l = new struct Layer;

		// Iterate on neurons for current layer
		for (int j = 0 ; j < numNeurons[i] ; j++) {
			struct Neuron* n = new struct Neuron;

			// Random weights with neurons of next layer
			if (i < numLayers - 1) { // If there is a next layer

				// Iterate on next layer neurons
				for (int k = 0 ; k < numNeurons[i+1] ; k++) {
					n->weights.push_back((float)rand() / (float)RAND_MAX);
				}
			}

			l->neurons.push_back(n);
		}

		n->layers.push_back(l);
	}
}

//----------------------------------------------------------------------
// Print weights in network
// @params :
//		- network
//----------------------------------------------------------------------
void print_weights(struct Network* n) {

	// Iterate on layers
	for (unsigned int i = 0 ; i < n->layers.size() ; i++) {
		printf("Layer %d :\n", i);
		
		struct Layer* l = n->layers.at(i);

		// Iterate on neurons
		for (unsigned int j = 0 ; j < l->neurons.size() ; j++) {
			printf("\tNeuron %d : \n", j);
		
			struct Neuron* neuron = l->neurons.at(j);

			// Iterate on weights
			for (unsigned int k = 0 ; k < neuron->weights.size() ; k++) {
				printf("\t\t%f\n", neuron->weights.at(k));
			}
		}
	}
}

//----------------------------------------------------------------------
// Sigmoid function
// @params :
//		- input
// @returns :
//		- output
//----------------------------------------------------------------------
float sigmoid(float x) {
	return 1 / (1 + exp(-x));
}

//----------------------------------------------------------------------
// Front propagation
// @params :
//		- neural network
//		- input values (same size as first layer)
// @returns :
//		- output values (same size as last layer)
//----------------------------------------------------------------------
std::vector<float> front_propagation(struct Network *n, std::vector<float> *input) {
	
	std::vector<std::vector<float> > internalResults;

	// Initialize internalResults
	for (unsigned int i = 1 ; i < n->layers.size() ; i++) { // Iterate on layers (BEGIN ON SECOND LAYER)
		std::vector<float> layerResults;
		
		struct Layer* l = n->layers.at(i);

		for (unsigned j = 0 ; j < l->neurons.size() ; j++) { // Iterate on neurons
			layerResults.push_back(0.0);
		}

		internalResults.push_back(layerResults);
	}

	// Compute internal results for first layer
	struct Layer *inputLayer = n->layers.at(0);
	struct Layer *currentOutputLayer = n->layers.at(1);
	for (unsigned i = 0 ; i < inputLayer->neurons.size() ; i++) { // Iterate on input layer neurons
		
		// Iterate on current output layer neurons
		for (unsigned int j = 0 ; j < currentOutputLayer->neurons.size() ; j++) {
			internalResults.at(0).at(j) += (input->at(i) * inputLayer->neurons.at(i)->weights.at(j));
		}
	}
	for (unsigned int i = 0 ; i < currentOutputLayer->neurons.size() ; i++) {
		internalResults.at(0).at(i) = sigmoid(internalResults.at(0).at(i));

		printf("%f\n", internalResults.at(0).at(i));
	}

	// Result output of last layer
	return internalResults.back();
}

int main (int argc, char *argv[]) {

	struct Network n;
	std::vector<float> input;

	srand(time(NULL));

	// Initialize network
	int numNeurons[3] = {2, 3, 2};
	init_network(&n, 3, numNeurons);

	print_weights(&n);

	printf("\n\n");

	input.push_back(0.6);
	input.push_back(0.8);

	front_propagation(&n, &input);

	return 0;
}
