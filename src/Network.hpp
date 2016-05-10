#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <vector>

struct Neuron {
	std::vector<float> weights; // Weights of connexions with next layer
	float bias;
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
void init_network(struct Network* n, int numLayers, int numNeurons[]);

#endif
