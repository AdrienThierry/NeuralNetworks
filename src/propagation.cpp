#include "propagation.hpp"

std::vector<std::vector<float> > front_propagation(struct Network *n, std::vector<float> *input) {
	
	std::vector<std::vector<float> > internalResults;

	// Initialize internalResults
	unsigned int numLayers = n->layers.size();
	for (unsigned int i = 1 ; i < numLayers ; ++i) { // Iterate on layers (BEGIN ON SECOND LAYER)
		internalResults.emplace_back(n->layers.at(i)->neurons.size());
	}

	// Compute internal results for first layer
	struct Layer *currentInputLayer = n->layers.at(0);
	struct Layer *currentOutputLayer = n->layers.at(1);
	unsigned int numNeuronsInput = currentInputLayer->neurons.size();
	unsigned int numNeuronsOutput = currentOutputLayer->neurons.size();

	for (unsigned i = 0 ; i < numNeuronsInput ; ++i) { // Iterate on input layer neurons
		
		float currentInput = input->at(i);

		// Iterate on current output layer neurons
		for (unsigned int j = 0 ; j < numNeuronsOutput ; ++j) {
			internalResults.at(0).at(j) += (currentInput * currentInputLayer->neurons.at(i)->weights.at(j));
		}
	}

	for (unsigned int i = 0 ; i < numNeuronsOutput ; ++i) {
		internalResults.at(0).at(i) = sigmoid(internalResults.at(0).at(i), currentOutputLayer->neurons.at(i)->bias);
	}

	// Compute remaining internal results
	for (unsigned int i = 1 ; i < numLayers - 1 ; ++i) { // Iterate on layers

		currentInputLayer = n->layers.at(i);
		currentOutputLayer = n->layers.at(i+1);

		numNeuronsInput = currentInputLayer->neurons.size();
		numNeuronsOutput = currentOutputLayer->neurons.size();

		for (unsigned j = 0 ; j < numNeuronsInput ; ++j) { // Iterate on input layer neurons
		
			// Iterate on current output layer neurons
			for (unsigned int k = 0 ; k < numNeuronsOutput ; ++k) {
				internalResults.at(i).at(k) += (internalResults.at(i-1).at(j) * currentInputLayer->neurons.at(j)->weights.at(k));
			}
		}

		for (unsigned int j = 0 ; j < numNeuronsOutput ; ++j) {
			internalResults.at(i).at(j) = sigmoid(internalResults.at(i).at(j), currentOutputLayer->neurons.at(j)->bias);
		}
	}

	return internalResults;
}

void back_propagation(	struct Network *n, std::vector<std::vector<float> > *internalResults, 
						std::vector<float> *inputs, std::vector<float> *desiredOutput, float speed) {
	
	std::vector<std::vector<float> > deltas;

	// Initialize deltas
	// NO DELTAS FOR FIRST LAYER, so deltas.at(0) is for SECOND layer
	unsigned int numLayers = n->layers.size();
	for (unsigned int i = 1 ; i < numLayers ; ++i) { // Iterate on layers
		deltas.emplace_back(n->layers.at(i)->neurons.size());
	}

	// Compute deltas for last layer
	std::vector<float> *lastLayerDeltas = &(deltas.back());
	struct Layer *lastLayer = n->layers.back();
	unsigned int numNeuronsLastLayer = lastLayer->neurons.size();
	for (unsigned int i = 0 ; i < numNeuronsLastLayer ; ++i) { // Iterate on neurons
		float currentOutput = internalResults->back().at(i);

		float currentDelta = 1.0;
		currentDelta *= currentOutput;
		currentDelta *= (1.0 - currentOutput);
		currentDelta *= (currentOutput - desiredOutput->at(i));

		lastLayerDeltas->at(i) = currentDelta;
	}

	// Compute deltas for remaining layers except first one
	for (unsigned int i = numLayers - 2 ; i > 0 ; --i) { // Iterate on layers
		struct Layer* currentLayer = n->layers.at(i);
		std::vector<float> *currentLayerDeltas = &(deltas.at(i-1));
		std::vector<float> *nextLayerDeltas = &(deltas.at(i));

		unsigned int numNeuronsCurrentLayer = currentLayer->neurons.size();
		unsigned int numNextLayerDeltas = nextLayerDeltas->size();

		for (unsigned int j = 0 ; j < numNeuronsCurrentLayer ; ++j) {

			struct Neuron* currentNeuron = currentLayer->neurons.at(j);

			// Compute sum of weighted next layer deltas
			float weightedSumNextLayerDeltas = 0.0;

			for (unsigned int k = 0 ; k < numNextLayerDeltas ; ++k) { // Iterate on next layer deltas
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
	unsigned int numNeuronsFirstLayer = firstLayer->neurons.size();
	std::vector<float> firstDeltas = deltas.at(0);
	for (unsigned int i = 0 ; i < numNeuronsFirstLayer ; ++i) {

		unsigned int numWeights = firstLayer->neurons.at(i)->weights.size();
		
		float iThInput = inputs->at(i);

		struct Neuron* iThNeuron = firstLayer->neurons.at(i);

		for (unsigned int j = 0 ; j < numWeights ; ++j) {
			float deltaW = -speed * firstDeltas.at(j) * iThInput;
			iThNeuron->weights.at(j) += deltaW;
		}
	}

	// Update weights for other layers (except last one because it has no next layer)
	for (unsigned int i = 1 ; i < numLayers - 1 ; ++i) {
		
		struct Layer* currentLayer = n->layers.at(i);
		unsigned int numNeuronsCurrentLayer = currentLayer->neurons.size();
		for (unsigned int j = 0 ; j < numNeuronsCurrentLayer ; ++j) {

			unsigned int numWeights = currentLayer->neurons.at(j)->weights.size();

			for (unsigned int k = 0 ; k < numWeights ; ++k) {
				float deltaW = -speed * deltas.at(i).at(k) * internalResults->at(i-1).at(j);
				currentLayer->neurons.at(j)->weights.at(k) += deltaW;
			}
		}
	}

	// Update bias
	for (unsigned int i = 1 ; i < numLayers ; ++i) {
		struct Layer* currentLayer = n->layers.at(i);
		unsigned int numNeuronsCurrentLayer = currentLayer->neurons.size();

		for (unsigned int j = 0 ; j < numNeuronsCurrentLayer ; ++j) {
			float deltaBias = -speed * deltas.at(i-1).at(j);
			currentLayer->neurons.at(j)->bias += deltaBias;
		}
	}
}
