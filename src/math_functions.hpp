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

#endif
