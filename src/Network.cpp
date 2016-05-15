#include "Network.hpp"

void init_network(struct Network* n, int numLayers, int numNeurons[]) {
	// Iterate on layers
	for (int i = 0 ; i < numLayers ; i++) {
		struct Layer* l = new struct Layer;

		// Iterate on neurons for current layer
		for (int j = 0 ; j < numNeurons[i] ; j++) {
			struct Neuron* neuron = new struct Neuron;

			// Random weights with neurons of next layer
			if (i < numLayers - 1) { // If there is a next layer

				// Iterate on next layer neurons
				for (int k = 0 ; k < numNeurons[i+1] ; k++) {
					neuron->weights.push_back(random_float(-0.5, 0.5));
				}
			}

			// Random bias
			neuron->bias = random_float(-0.5, 0.5);

			l->neurons.push_back(neuron);
		}

		n->layers.push_back(l);
	}
}
