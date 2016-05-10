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

#define MAX_ERROR 0.1
#define MIN_GOOD 10 // Number of successive times the network must achieve the max error

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
		input.push_back(rand() % 2);

		std::vector<std::vector<float> > result = front_propagation(&n, &input);

		error = compute_error(&(result.back()), &input);
		//printf("Error = %f\n", error);
		back_propagation(&n, &result, &input, &input, 5.0);
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
