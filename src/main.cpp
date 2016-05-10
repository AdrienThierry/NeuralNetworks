#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cmath>
#include <vector>

#include "propagation.hpp"
#include "Network.hpp"
#include "math_functions.hpp"
#include "utility.hpp"

#define MAX_ERROR 0.001
#define MIN_GOOD 10 // Number of successive times the network must achieve an error below the max error

int main (int argc, char *argv[]) {

	struct Network n;

	srand(time(NULL));

	// Initialize network
	int numNeurons[3] = {1, 1, 1}; // 1 neuron per layer
	init_network(&n, 3, numNeurons);

	// Training loop
	float error;
	int numIterations = 0;
	int numGood = 0;
	do {
		std::vector<float> input;

//		input.push_back(rand() % 2); // Random value in {0,1}
		input.push_back((float)rand() / (float)RAND_MAX);

		std::vector<std::vector<float> > result = front_propagation(&n, &input);

		error = compute_error(&(result.back()), &input);
		back_propagation(&n, &result, &input, &input, 1.0);
		numIterations++;

		if (error < MAX_ERROR)
			numGood++;
		else
			numGood = 0;

	} while (numGood < MIN_GOOD);

	printf("\nNumber of iterations : %d\n", numIterations);
	
	// Let user test the trained network
	float input;
	int roundedResult;
	while(1) {
		scanf("%f", &input);
		std::vector<float> inputVector;
		inputVector.push_back(input);

		std::vector<std::vector<float> > result = front_propagation(&n, &inputVector);
		
//		if (result.back().at(0) > 0.5)
//			roundedResult = 1;
//		else
//			roundedResult = 0;

//		printf("Result : %d\n", roundedResult);

		printf("Result : %f\n", result.back().at(0));
	}

	return 0;
}
