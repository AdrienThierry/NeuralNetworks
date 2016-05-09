#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cmath>
#include <vector>

#define MAX_ERROR 0.001
#define MIN_GOOD 10

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
					neuron->weights.push_back((float)rand() / (float)RAND_MAX);
				}
			}

			// Random bias
			neuron->bias = (float)rand() / (float)RAND_MAX;

			l->neurons.push_back(neuron);
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

//----------------------------------------------------------------------
// Sigmoid function
// @params :
//		- input
// @returns :
//		- output
//----------------------------------------------------------------------
float sigmoid(float x, float bias) {
	return 1 / (1 + exp(-(x+bias)));
}

//----------------------------------------------------------------------
// Front propagation
// @params :
//		- neural network
//		- input values (same size as first layer)
// @returns :
//		- all output values in network (first coord is layer, second is
//		neuron output)
//----------------------------------------------------------------------
std::vector<std::vector<float> > front_propagation(struct Network *n, std::vector<float> *input) {
	
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
	struct Layer *currentInputLayer = n->layers.at(0);
	struct Layer *currentOutputLayer = n->layers.at(1);
	for (unsigned i = 0 ; i < currentInputLayer->neurons.size() ; i++) { // Iterate on input layer neurons
		
		// Iterate on current output layer neurons
		for (unsigned int j = 0 ; j < currentOutputLayer->neurons.size() ; j++) {
			internalResults.at(0).at(j) += (input->at(i) * currentInputLayer->neurons.at(i)->weights.at(j));
		}
	}
	for (unsigned int i = 0 ; i < currentOutputLayer->neurons.size() ; i++) {
		internalResults.at(0).at(i) = sigmoid(internalResults.at(0).at(i), currentOutputLayer->neurons.at(i)->bias);
	}

	// Compute remaining internal results
	for (unsigned int i = 1 ; i < n->layers.size() - 1 ; i++) { // Iterate on layers

		currentInputLayer = n->layers.at(i);
		currentOutputLayer = n->layers.at(i+1);

		for (unsigned j = 0 ; j < currentInputLayer->neurons.size() ; j++) { // Iterate on input layer neurons
		
			// Iterate on current output layer neurons
			for (unsigned int k = 0 ; k < currentOutputLayer->neurons.size() ; k++) {
				internalResults.at(i).at(k) += (internalResults.at(i-1).at(j) * currentInputLayer->neurons.at(j)->weights.at(k));
			}
		}
		for (unsigned int j = 0 ; j < currentOutputLayer->neurons.size() ; j++) {
			internalResults.at(i).at(j) = sigmoid(internalResults.at(i).at(j), currentOutputLayer->neurons.at(j)->bias);
		}
	}

	return internalResults;
}

//----------------------------------------------------------------------
// Compute error
// @params :
//		- output values
//		- desired output values
// @returns :
//		- error
//----------------------------------------------------------------------
float compute_error(std::vector<float> *output, std::vector<float> *desiredOutput) {
	
	float result = 0.0;

	// Check that output and desiredOutput have same length
	if (output->size() != desiredOutput->size()) {
		printf("computeError ERROR : output and desiredOutput must have same size\n");
		return -1;
	}

	for (unsigned int i = 0 ; i < output->size() ; i++) {
		result += ((desiredOutput->at(i) - output->at(i)) * (desiredOutput->at(i) - output->at(i)));
	}

	return result;
}

//----------------------------------------------------------------------
// Back propagation
// @params :
//		- neural network
//		- internal outputs in network
//		- input values
//		- desired output
//		- speed factor
//----------------------------------------------------------------------
void back_propagation(	struct Network *n, std::vector<std::vector<float> > *internalResults, 
						std::vector<float> *inputs, std::vector<float> *desiredOutput, float speed) {
	
	std::vector<std::vector<float> > deltas;

	// Initialize deltas
	for (unsigned int i = 1 ; i < n->layers.size() ; i++) { // Iterate on layers
		std::vector<float> currentDeltas;

		struct Layer* l = n->layers.at(i);

		// Iterate on neurons in layer
		for (unsigned int j = 0 ; j < l->neurons.size() ; j++) {
			currentDeltas.push_back(0.0);
		}

		deltas.push_back(currentDeltas);
	}

	// Compute deltas for last layer
	std::vector<float> *lastLayerDeltas = &(deltas.back());
	struct Layer *lastLayer = n->layers.back();
	for (unsigned int i = 0 ; i < lastLayer->neurons.size() ; i++) { // Iterate on neurons
		float currentDelta = 1.0;
		currentDelta *= internalResults->back().at(i);
		currentDelta *= (1.0 - internalResults->back().at(i));
		currentDelta *= (internalResults->back().at(i) - desiredOutput->at(i));

		lastLayerDeltas->at(i) = currentDelta;
	}

	// Compute deltas for remaining layers except first one
	for (unsigned int i = n->layers.size() - 2 ; i > 0 ; i--) { // Iterate on layers
		struct Layer* currentLayer = n->layers.at(i);
		std::vector<float> *currentLayerDeltas = &(deltas.at(i-1));
		std::vector<float> *nextLayerDeltas = &(deltas.at(i));

		for (unsigned int j = 0 ; j < currentLayer->neurons.size() ; j++) {

			struct Neuron* currentNeuron = currentLayer->neurons.at(j);

			// Compute sum of weighted next layer deltas
			float weightedSumNextLayerDeltas = 0.0;

			for (unsigned int k = 0 ; k < nextLayerDeltas->size() ; k++) { // Iterate on next layer deltas
				weightedSumNextLayerDeltas += (nextLayerDeltas->at(k) * currentNeuron->weights.at(k));			
			}

			float currentDelta = 1.0;
			currentDelta *= internalResults->at(i-1).at(j);
			currentDelta *= (1.0 - internalResults->at(i-1).at(j));
			currentDelta *= weightedSumNextLayerDeltas;

			currentLayerDeltas->at(j) = currentDelta;
		}
	}

	// Update weights for first layer
	struct Layer* firstLayer = n->layers.at(0);
	for (unsigned int i = 0 ; i < firstLayer->neurons.size() ; i++) {

		for (unsigned int j = 0 ; j < firstLayer->neurons.at(i)->weights.size() ; j++) {
			float deltaW = -speed * deltas.at(0).at(j) * inputs->at(i);
			firstLayer->neurons.at(i)->weights.at(j) += deltaW;
		}
	}

	// Update weights for other layers (except last one)
	for (unsigned int i = 1 ; i < n->layers.size() - 1 ; i++) {
		
		struct Layer* currentLayer = n->layers.at(i);
		for (unsigned int j = 0 ; j < currentLayer->neurons.size() ; j++) {

			for (unsigned int k = 0 ; k < currentLayer->neurons.at(j)->weights.size() ; k++) {
				float deltaW = -speed * deltas.at(i).at(k) * internalResults->at(i-1).at(k);
				currentLayer->neurons.at(j)->weights.at(k) += deltaW;
			}
		}
	}

	// Update bias
	for (unsigned int i = 1 ; i < n->layers.size() ; i++) {
		struct Layer* currentLayer = n->layers.at(i);

		for (unsigned int j = 0 ; j < currentLayer->neurons.size() ; j++) {
			float deltaBias = -speed * deltas.at(i-1).at(j);
			currentLayer->neurons.at(j)->bias += deltaBias;
		}
	}
}

//----------------------------------------------------------------------
// Print float vector
// @params :
//		- float vector
//----------------------------------------------------------------------
void print_float_vector(std::vector<float> *vector) {
	printf("[");

	for (unsigned int i = 0 ; i < vector->size() ; i++) {
		printf("%f", vector->at(i));
		if (i < vector->size() - 1)
			printf(" ");
	}

	printf("]\n");
}

int main (int argc, char *argv[]) {

	struct Network n;

	srand(time(NULL));

	// Initialize network
	int numNeurons[3] = {1, 1, 1};
	init_network(&n, 3, numNeurons);

	// Training loop
	float error;
	int numIterations = 0;
	int numGood = 0;
	do {
		std::vector<float> input;
		input.push_back((float)rand() / (float)RAND_MAX);

		std::vector<std::vector<float> > result = front_propagation(&n, &input);
		error = compute_error(&(result.back()), &input);
		//printf("Error = %f\n", error);
		back_propagation(&n, &result, &input, &input, 0.01);
		numIterations++;

		//print_float_vector(&(input));
		//print_float_vector(&(result.back()));

		if (error < MAX_ERROR)
			numGood++;
		else
			numGood = 0;

	} while (numGood < MIN_GOOD);

	printf("\nNumber of iterations : %d\n", numIterations);
	
	// Let user test the trained network
	float input;
	while(1) {
		scanf("%f", &input);
		std::vector<float> inputVector;
		inputVector.push_back(input);

		std::vector<std::vector<float> > result = front_propagation(&n, &inputVector);
		print_float_vector(&(result.back()));
	}

	return 0;
}
