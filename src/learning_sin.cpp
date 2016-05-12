#include "learning_sin.hpp"

void learning_sin() {
	
	struct Network n;

	// Initialize network
	int numNeurons[3] = {1, 20, 1};
	init_network(&n, 3, numNeurons);

	// Create sin values
	std::vector<float> sinX;
	std::vector<float> sinY;
	for (int i = 0 ; i < TOTAL_NUM_VALUES ; i++) {
		float x = random_float(MIN_X, MAX_X);
		sinX.push_back(x);
		sinY.push_back(0.5*(sin(x)+1.0)); // sin(x) normalized between 0 and 1
	}

	// Training loop
	float error;
	int numIterations = 0;
	int numGood = 0;
	std::vector<float> errorsY;

	do {
		std::vector<float> input;
		std::vector<float> desiredOutput;

		input.push_back(sinX.at(numIterations));
		desiredOutput.push_back(sinY.at(numIterations));

		std::vector<std::vector<float> > result = front_propagation(&n, &input);

		error = compute_error(&(result.back()), &desiredOutput);
		errorsY.push_back(error);
		back_propagation(&n, &result, &input, &desiredOutput, 0.1);
		numIterations++;

		if (error < MAX_ERROR)
			numGood++;
		else
			numGood = 0;

	} while (numGood < MIN_GOOD && (unsigned int)numIterations < sinX.size());

	printf("\nNumber of iterations : %d\n", numIterations);

	// Make vector with error values
	std::vector<float> errorsX;
	for (unsigned int i = 0 ; i < errorsY.size() ; i++) {
		errorsX.push_back(i);
	}
	std::vector<std::vector<float> > values;
	std::vector<std::string> titles;
	values.push_back(errorsX);
	values.push_back(errorsY);
	titles.push_back("Error");
	plot(values, titles);
	
	// Let user test the trained network
	float input;
	while(1) {
		scanf("%f", &input);
		std::vector<float> inputVector;
		inputVector.push_back(input);

		std::vector<std::vector<float> > result = front_propagation(&n, &inputVector);

		printf("Result : %f\n", (result.back().at(0)-0.5)/0.5);
	}
}
