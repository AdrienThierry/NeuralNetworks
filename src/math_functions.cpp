#include "math_functions.hpp"

float sigmoid(float x, float bias) {
	return 1 / (1 + exp(-(x+bias)));
}

float compute_error(std::vector<float> *output, std::vector<float> *desiredOutput) {
	
	float result = 0.0;

	// Check that output and desiredOutput have same length
	if (output->size() != desiredOutput->size()) {
		print_error("compute_error", "output and desiredOutput must have same size\n");
		return -1;
	}

	for (unsigned int i = 0 ; i < output->size() ; i++) {
		result += ((desiredOutput->at(i) - output->at(i)) * (desiredOutput->at(i) - output->at(i)));
	}

	return result;
}
