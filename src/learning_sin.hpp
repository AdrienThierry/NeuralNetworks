#ifndef LEARNING_SIN_H
#define LEARNING_SIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <vector>
#include <string>

#include "utility.hpp"
#include "propagation.hpp"
#include "plot.hpp"
#include "Network.hpp"

#define MAX_ERROR 0.0001
#define MIN_GOOD 100 // Number of successive times the network must achieve an error below the max error
#define TOTAL_NUM_VALUES 30000 // Total number of values in data set (training + validation + test)
#define MIN_X 0.0
#define MAX_X 10

void learning_sin();

#endif
