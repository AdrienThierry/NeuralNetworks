#include "math_functions.hpp"

float sigmoid(float x, float bias) {
	return 1 / (1 + exp(-(x+bias)));
}

float compute_error(std::vector<float> *output, std::vector<float> *desiredOutput) {
	
	float result = 0.0;

	// Check that output and desiredOutput have same length
	if (output->size() != desiredOutput->size()) {
		print_error("compute_error", "output and desiredOutput must have same size");
		return -1;
	}

	for (unsigned int i = 0 ; i < output->size() ; i++) {
		result += ((desiredOutput->at(i) - output->at(i)) * (desiredOutput->at(i) - output->at(i)));
	}

	result /= 2.0;

	return result;
}

std::vector<float> compute_mean_error(std::vector<float> error, int window) {
	std::vector<float> result;

	// Check that error vector size is bigger than window
	if (error.size() <= (unsigned int)window) {
		print_error("compute_mean_error", "error vector size must be bigger than window");
		return result;
	}

	for (unsigned int i = 0 ; i < error.size() - window ; i++) {
		float tmp = 0.0;

		for (int j = 0 ; j < window ; j++) {
			tmp += error.at(i+j);
		}

		tmp /= (float)window;

		result.push_back(tmp);
	}

	for (unsigned int i = error.size() - window ; i < error.size() ; i++) {
		result.push_back(result.back());
	}

	return result;

}
