#include "mnist.hpp"

void learning_mnist(std::string trainingImagesPath, std::string trainingLabelsPath, std::string testImagesPath, std::string testLabelsPath) {
	struct Network n;

	int numTrainingImages;
	int numTrainingLabels;
	int numTestImages;
	int numTestLabels;
	int imageSize;

	std::vector<std::vector<float> > trainingInputs;
	std::vector<std::vector<float> > trainingDesiredOutputs;
	std::vector<std::vector<float> > testInputs;
	std::vector<std::vector<float> > testDesiredOutputs;

	// Read data files
	uchar** trainingImages = read_mnist_images(trainingImagesPath, numTrainingImages, imageSize);
	uchar** testImages = read_mnist_images(testImagesPath, numTestImages, imageSize);
	uchar* trainingLabels = read_mnist_labels(trainingLabelsPath, numTrainingLabels);
	uchar* testLabels = read_mnist_labels(testLabelsPath, numTestLabels);

	// Initialize network
	int numNeurons[3] = {imageSize, NUM_HIDDEN_NEURONS, 10};
	init_network(&n, 3, numNeurons);

	// Create training vectors
	for (int i = 0 ; i < numTrainingImages ; i++) {
		std::vector<float> currentInput;
		std::vector<float> currentDesiredOutput;

		for (int j = 0 ; j < imageSize ; j++) {
			currentInput.push_back((float)trainingImages[i][j] / 255.0);
		}

		for (int j = 0 ; j < 10 ; j++) {
			if (j == trainingLabels[i])
				currentDesiredOutput.push_back(1.0);
			else
				currentDesiredOutput.push_back(0.0);
		}

		trainingInputs.push_back(currentInput);
		trainingDesiredOutputs.push_back(currentDesiredOutput);
	}

	// Training loop
	float error;
	float speed = 1.0;
	int percent = 0;
	std::vector<float> errorsY;
	for (int i = 0 ; i < numTrainingImages ; i++) {
		std::vector<std::vector<float> > result = front_propagation(&n, &(trainingInputs.at(i)));

		error = compute_error(&(result.back()), &(trainingDesiredOutputs.at(i)));
		errorsY.push_back(error);
		back_propagation(&n, &result, &(trainingInputs.at(i)), &(trainingDesiredOutputs.at(i)), speed);

		if ((int)((float)i / (float)numTrainingImages * 100) > percent) {
			percent = (int)((float)i / (float)numTrainingImages * 100);
			printf("%d %%\n", percent);
		}
	}

	// Compute and plot mean error
	std::vector<float> errorsX;
	for (unsigned int i = 0 ; i < errorsY.size() ; i++) {
		errorsX.push_back(i);
	}
	std::vector<std::vector<float> > values;
	std::vector<std::string> titles;
	values.push_back(errorsX);
	std::vector<float> meanErrorsY = compute_mean_error(errorsY, 100);
	values.push_back(meanErrorsY);
	titles.push_back("Training error");
	printf("Final error : %f\n", meanErrorsY.back());
	plot(values, titles);

	// Create test vectors
	for (int i = 0 ; i < numTestImages ; i++) {
		std::vector<float> currentInput;
		std::vector<float> currentDesiredOutput;

		for (int j = 0 ; j < imageSize ; j++) {
			currentInput.push_back((float)testImages[i][j] / 255.0);
		}

		for (int j = 0 ; j < 10 ; j++) {
			if (j == testLabels[i])
				currentDesiredOutput.push_back(1.0);
			else
				currentDesiredOutput.push_back(0.0);
		}

		testInputs.push_back(currentInput);
		testDesiredOutputs.push_back(currentDesiredOutput);
	}

	// Test loop
	percent = 0;
	for (int i = 0 ; i < numTestImages ; i++) {
		std::vector<std::vector<float> > result = front_propagation(&n, &(testInputs.at(i)));

		error = compute_error(&(result.back()), &(testDesiredOutputs.at(i)));

		if (i % 100 == 0) {
			printf("Test %d/%d : error = %f\n", i, numTestImages, error);
			std::string path = "result/";
			path += std::to_string(decode_output(&(result.back())));
			path += "_";
			path += std::to_string(i);
			write_pgm_image(path, testImages[i], imageSize);
		}
			
	}
}

uchar** read_mnist_images(std::string full_path, int& number_of_images, int& image_size) {
    auto reverseInt = [](int i) {
        unsigned char c1, c2, c3, c4;
        c1 = i & 255, c2 = (i >> 8) & 255, c3 = (i >> 16) & 255, c4 = (i >> 24) & 255;
        return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
    };

    typedef unsigned char uchar;

    std::ifstream file(full_path);

    if(file.is_open()) {
        int magic_number = 0, n_rows = 0, n_cols = 0;

        file.read((char *)&magic_number, sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        if(magic_number != 2051) print_error("read_mnist_images", "Invalid MNIST image file!");

        file.read((char *)&number_of_images, sizeof(number_of_images)), number_of_images = reverseInt(number_of_images);
        file.read((char *)&n_rows, sizeof(n_rows)), n_rows = reverseInt(n_rows);
        file.read((char *)&n_cols, sizeof(n_cols)), n_cols = reverseInt(n_cols);

        image_size = n_rows * n_cols;

        uchar** _dataset = new uchar*[number_of_images];
        for(int i = 0; i < number_of_images; i++) {
            _dataset[i] = new uchar[image_size];
            file.read((char *)_dataset[i], image_size);
        }
        return _dataset;
    } else {
        print_error("read_mnist_images", "Cannot open file `" + full_path + "`!");
		return NULL;
    }
}

uchar* read_mnist_labels(std::string full_path, int& number_of_labels) {
    auto reverseInt = [](int i) {
        unsigned char c1, c2, c3, c4;
        c1 = i & 255, c2 = (i >> 8) & 255, c3 = (i >> 16) & 255, c4 = (i >> 24) & 255;
        return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
    };

    typedef unsigned char uchar;

    std::ifstream file(full_path);

    if(file.is_open()) {
        int magic_number = 0;
        file.read((char *)&magic_number, sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        if(magic_number != 2049) print_error("read_mnist_labels", "Invalid MNIST label file!");

        file.read((char *)&number_of_labels, sizeof(number_of_labels)), number_of_labels = reverseInt(number_of_labels);

        uchar* _dataset = new uchar[number_of_labels];
        for(int i = 0; i < number_of_labels; i++) {
            file.read((char*)&_dataset[i], 1);
        }
        return _dataset;
    } else {
        print_error("read_mnist_labels", "Unable to open file `" + full_path + "`!");
		return NULL;
    }
}

int decode_output(std::vector<float> *output) {

	int indexMax = 0;
	float max = output->at(0);

	for (unsigned int i = 1 ; i < output->size() ; i++) {
		if (output->at(i) > max) {
			max = output->at(i);
			indexMax = i;
		}
	}

	return indexMax;
}
