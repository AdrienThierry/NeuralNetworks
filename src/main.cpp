#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string>

#include "propagation.hpp"
#include "Network.hpp"
#include "math_functions.hpp"
#include "utility.hpp"
#include "plot.hpp"
#include "learning_sin.hpp"
#include "mnist.hpp"

int main (int argc, char *argv[]) {

	srand(time(NULL));

	//learning_sin();
	learning_mnist("data/train-images.idx3-ubyte", "data/train-labels.idx1-ubyte", "data/t10k-images.idx3-ubyte", "data/t10k-labels.idx1-ubyte");

	return 0;
}
