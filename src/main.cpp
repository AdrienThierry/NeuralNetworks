#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "propagation.hpp"
#include "Network.hpp"
#include "math_functions.hpp"
#include "utility.hpp"
#include "plot.hpp"
#include "learning_sin.hpp"

int main (int argc, char *argv[]) {

	srand(time(NULL));

	learning_sin();

	return 0;
}
