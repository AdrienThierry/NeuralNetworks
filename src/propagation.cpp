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
	for (unsigned int i = 1 ; i < n->layers.size() ; ++i) { // Iterate on layers
		std::vector<float> currentDeltas;

		struct Layer* l = n->layers.at(i);

		// Iterate on neurons in layer
		for (unsigned int j = 0 ; j < l->neurons.size() ; ++j) {
			currentDeltas.push_back(0.0);
		}

		deltas.push_back(currentDeltas);
	}

	// Compute deltas for last layer
	std::vector<float> *lastLayerDeltas = &(deltas.back());
	struct Layer *lastLayer = n->layers.back();
	for (unsigned int i = 0 ; i < lastLayer->neurons.size() ; ++i) { // Iterate on neurons
		float currentDelta = 1.0;
		currentDelta *= internalResults->back().at(i);
		currentDelta *= (1.0 - internalResults->back().at(i));
		currentDelta *= (internalResults->back().at(i) - desiredOutput->at(i));

		lastLayerDeltas->at(i) = currentDelta;
	}

	// Compute deltas for remaining layers except first one
	// TODO : parcours dans l'autre sens ?
	for (unsigned int i = n->layers.size() - 2 ; i > 0 ; --i) { // Iterate on layers
		struct Layer* currentLayer = n->layers.at(i);
		std::vector<float> *currentLayerDeltas = &(deltas.at(i-1));
		std::vector<float> *nextLayerDeltas = &(deltas.at(i));

		for (unsigned int j = 0 ; j < currentLayer->neurons.size() ; ++j) {

			struct Neuron* currentNeuron = currentLayer->neurons.at(j);

			// Compute sum of weighted next layer deltas
			float weightedSumNextLayerDeltas = 0.0;

			for (unsigned int k = 0 ; k < nextLayerDeltas->size() ; ++k) { // Iterate on next layer deltas
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
	for (unsigned int i = 0 ; i < firstLayer->neurons.size() ; ++i) {

		for (unsigned int j = 0 ; j < firstLayer->neurons.at(i)->weights.size() ; ++j) {
			float deltaW = -speed * deltas.at(0).at(j) * inputs->at(i);
			firstLayer->neurons.at(i)->weights.at(j) += deltaW;
		}
	}

	// Update weights for other layers (except last one because it has no next layer)
	for (unsigned int i = 1 ; i < n->layers.size() - 1 ; ++i) {
		
		struct Layer* currentLayer = n->layers.at(i);
		for (unsigned int j = 0 ; j < currentLayer->neurons.size() ; ++j) {

			for (unsigned int k = 0 ; k < currentLayer->neurons.at(j)->weights.size() ; ++k) {
				float deltaW = -speed * deltas.at(i).at(k) * internalResults->at(i-1).at(j);
				currentLayer->neurons.at(j)->weights.at(k) += deltaW;
			}
		}
	}

	// Update bias
	for (unsigned int i = 1 ; i < n->layers.size() ; ++i) {
		struct Layer* currentLayer = n->layers.at(i);

		for (unsigned int j = 0 ; j < currentLayer->neurons.size() ; ++j) {
			float deltaBias = -speed * deltas.at(i-1).at(j);
			currentLayer->neurons.at(j)->bias += deltaBias;
		}
	}
}
