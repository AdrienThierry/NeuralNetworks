#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
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

	if (argc < 2) {
		print_error("main", "too few arguments");
	}

	else if (argc > 2) {
		print_error("main", "too many arguments");
	}

	else {

		if (strcmp(argv[1], "--learning-sin") == 0) {
			learning_sin();
		}

		else if (strcmp(argv[1], "--learning-mnist") == 0) {
			learning_mnist("data/train-images.idx3-ubyte", "data/train-labels.idx1-ubyte", "data/t10k-images.idx3-ubyte", "data/t10k-labels.idx1-ubyte");
		}

		else {
			print_error("main", "wrong argument (choose --learning-sin or --learning-mnist)");
		}
	}

	return 0;
}
