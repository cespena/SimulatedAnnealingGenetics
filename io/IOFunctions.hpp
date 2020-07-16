#ifndef IO_FUNCTIONS
#define IO_FUNCTIONS

#include <iostream>
#include <cstring>
#include <fstream>
#include <stdexcept>

#include "../matrix/AllMatrices.hpp"

struct CommandLineInfo
{
	unsigned long int iterations;
	std::string G_output;
	std::string E_output;

	CommandLineInfo()
	{
		iterations = 1e6;
		G_output = "G_results";
		E_output = "E_results";
	}
};

////Input////
//Gets command line arguments for the program.
CommandLineInfo get_info(int argc, char* argv[], AllMatrices& am);

////Output////
//Outputs G and E matrices to separate files.
void print_results(std::pair<Matrix, Matrix>& results, std::string G_filename, std::string E_filename);

#endif //IO_FUNCTIONS
