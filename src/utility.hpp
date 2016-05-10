#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <vector>
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

#endif
