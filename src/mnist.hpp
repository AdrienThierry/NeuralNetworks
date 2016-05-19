#ifndef MNIST_H
#define MNIST_H

#include <fstream>
#include <string>
#include <vector>

#include "utility.hpp"
#include "propagation.hpp"
#include "plot.hpp"
#include "Network.hpp"

#define NUM_HIDDEN_NEURONS 300

typedef unsigned char uchar;

//----------------------------------------------------------------------
// Example of neural network usage for learning MNIST handwritten
// numbers
// @params :
//		- path of the training images binary file
//		- path of the training labels binary file
//		- path of the test images binary file
//		- path of the test labels binary file
//----------------------------------------------------------------------
void learning_mnist(std::string trainingImagesPath, std::string trainingLabelsPath, std::string testImagesPath, std::string testLabelsPath);

//----------------------------------------------------------------------
// Read MNIST images binary file
// @params :
//		- path of the images binary file
//		- reference to the number of images (set by read_mnist_images)
//		- reference to the size of the images (cols*rows) (set by read_mnist_images)
// @returns :
//		- array of images (array of uchar*) or NULL if failed
//----------------------------------------------------------------------
uchar** read_mnist_images(std::string full_path, int& number_of_images, int& image_size);

//----------------------------------------------------------------------
// Read MNIST images binary file
// @params :
//		- path of the labels binary file
//		- reference to the number of labels (set by read_mnist_labels)
// @returns :
//		- array of labels (array of uchar) or NULL if failed
//----------------------------------------------------------------------
uchar* read_mnist_labels(std::string full_path, int& number_of_labels);

//----------------------------------------------------------------------
// Decode neural net output : output number is the index of the
// output node that has maximum value (one-hot encoding)
// @params :
//		- pointer to the neural net output vector
// @returns :
//		- output value (between 0 and 9)
//----------------------------------------------------------------------
int decode_output(std::vector<float>* output);

#endif
