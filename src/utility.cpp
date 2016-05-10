#include "utility.hpp"

void print_weights(struct Network* n) {

	// Iterate on layers
	for (unsigned int i = 0 ; i < n->layers.size() - 1 ; i++) {
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

void print_float_vector(std::vector<float> *vector) {
	printf("[");

	for (unsigned int i = 0 ; i < vector->size() ; i++) {
		printf("%f", vector->at(i));
		if (i < vector->size() - 1)
			printf(" ");
	}

	printf("]\n");
}
