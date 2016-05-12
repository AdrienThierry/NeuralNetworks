#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include "Network.hpp"

//----------------------------------------------------------------------
// Print weights in network
// @params :
//		- network
//----------------------------------------------------------------------
void print_weights(struct Network* n);

//----------------------------------------------------------------------
// Print float vector
// @params :
//		- float vector
//----------------------------------------------------------------------
void print_float_vector(std::vector<float> *vector);

//----------------------------------------------------------------------
// Print error
// @params :
//		- name of the function in which the error occured
//		- error message
//----------------------------------------------------------------------
void print_error(std::string functionName, std::string errorMessage);

//----------------------------------------------------------------------
// Generate a random float between two float values
// @params :
//		- min value
//		- max value
// @returns :
//		- random value between min and max
//----------------------------------------------------------------------
float random_float(float a, float b);

#endif
