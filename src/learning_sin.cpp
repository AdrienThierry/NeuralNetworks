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
	float speed = 1.0;
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
		back_propagation(&n, &result, &input, &desiredOutput, speed);
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
	std::vector<float> meanErrorsY = compute_mean_error(errorsY, MIN_GOOD);
	//values.push_back(errorsY);
	values.push_back(meanErrorsY);
	titles.push_back("Error");

	printf("Final error : %f\n", meanErrorsY.back());

	plot(values, titles);
	
	// Test of the network
//	std::vector<float> sinTestX;
//	std::vector<float> sinTestYReal;
//	std::vector<float> sinTestYNN;

//	for (int i = 0 ; i < TOTAL_NUM_VALUES ; i++) {
//		float x = MIN_X + i * (MAX_X - MIN_X) / TOTAL_NUM_VALUES;
//		
//		sinTestX.push_back(x);
//		sinTestYReal.push_back(sin(x));

//		std::vector<float> input;
//		input.push_back(sinTestX.at(i));
//		std::vector<std::vector<float> > result = front_propagation(&n, &input);

//		sinTestYNN.push_back((result.back().back() - 0.5)/0.5);

//	}
//	
//	std::vector<std::vector<float> > values;
//	std::vector<std::string> titles;
//	values.push_back(sinTestX);
//	values.push_back(sinTestYReal);
//	values.push_back(sinTestYNN);
//	titles.push_back("Real");
//	titles.push_back("NN");
//	plot(values, titles);

}
