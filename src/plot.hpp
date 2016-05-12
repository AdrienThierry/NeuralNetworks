#ifndef PLOT_H
#define PLOT_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include "utility.hpp"

//----------------------------------------------------------------------
// Plot a graph using gnuplot
// @params :
//		- values. 1 vector<float> for x values, and n vector<float>
//		for all the curves you want to plot
//		Each inner vector MUST HAVE the same size
//		- titles. 1 for each curve
//	@returns :
//		- 0 if success, -1 if failure
//----------------------------------------------------------------------
int plot(std::vector<std::vector<float> > values, std::vector<std::string> titles);

#endif
