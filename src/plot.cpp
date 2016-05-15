#include "plot.hpp"

int plot(std::vector<std::vector<float> > values, std::vector<std::string> titles) {
	
	FILE* gnuplotData = NULL;
	FILE* gnuplotScript = NULL;

	// Check if values and titles have correct sizes
	if (values.size() != titles.size() + 1) {
		print_error("plot", "size of titles must be size of values minus 1");

		return -1;
	}

	// Create file with data and command script
	gnuplotData = fopen("gnuplotData.txt", "w+");
	gnuplotScript = fopen("gnuplotScript.sh", "w+");

	// Fill script file
	fprintf(gnuplotScript, "#! /usr/bin/gnuplot\nplot ");
	for (unsigned int i = 0 ; i < titles.size() ; i++) {
		fprintf(gnuplotScript, "\"gnuplotData.txt\" using 1:%d title \"%s\" with lines", i + 2, titles.at(i).c_str());

		if (i < titles.size() - 1) {
			fprintf(gnuplotScript, ", ");
		}
		else {
			fprintf(gnuplotScript, "\npause -1");
		}
	}

	// Fill data file
	for (unsigned int j = 0 ; j < values.at(0).size() ; j++) {
		for (unsigned int i = 0 ; i < values.size() ; i++) {
			fprintf(gnuplotData, "%f\t", values.at(i).at(j));
		}
		fprintf(gnuplotData, "\n");
	}

	printf("Press enter to continue\n");

	// Close temporary files
	fclose(gnuplotData);
	fclose(gnuplotScript);

	// Execute script
	system("chmod +x ./gnuplotScript.sh");
	system("./gnuplotScript.sh");

	// Remove temporary files
	remove("gnuplotData.txt");
	remove("gnuplotScript.sh");

	return 0;

}
