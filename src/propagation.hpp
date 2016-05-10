#ifndef PROPAGATION_H
#define PROPAGATION_H

#include <vector>
#include "Network.hpp"
#include "math_functions.hpp"

//----------------------------------------------------------------------
// Front propagation
// @params :
//		- neural network
//		- input values (same size as first layer)
// @returns :
//		- all output values in network (first coord is layer, second is
//		neuron output)
//----------------------------------------------------------------------
std::vector<std::vector<float> > front_propagation(struct Network *n, std::vector<float> *input);

//----------------------------------------------------------------------
// Back propagation
// @params :
//		- neural network
//		- internal outputs in network
//		- input values
//		- desired output
//		- speed factor
//----------------------------------------------------------------------
void back_propagation(	struct Network *n, std::vector<std::vector<float> > *internalResults, 
						std::vector<float> *inputs, std::vector<float> *desiredOutput, float speed);

#endif
