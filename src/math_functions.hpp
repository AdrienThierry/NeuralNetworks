#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include <stdio.h>
#include <vector>
#include <cmath>

#include "utility.hpp"

//----------------------------------------------------------------------
// Sigmoid function
// @params :
//		- input
// @returns :
//		- output
//----------------------------------------------------------------------
float sigmoid(float x, float bias);

//----------------------------------------------------------------------
// Compute error
// @params :
//		- output values
//		- desired output values
// @returns :
//		- error
//----------------------------------------------------------------------
float compute_error(std::vector<float> *output, std::vector<float> *desiredOutput);

//----------------------------------------------------------------------
// Compute mean error ("windowed" error)
// @params :
//		- vector with error values
//		- window
// @returns :
//		- vector with mean error values (same size as input vector)
//----------------------------------------------------------------------
std::vector<float> compute_mean_error(std::vector<float> error, int window);
#endif
