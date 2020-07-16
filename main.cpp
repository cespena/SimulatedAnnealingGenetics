#include <iostream>
#include <string>

#include "matrix/Matrix.hpp"
#include "matrix/AllMatrices.hpp"
#include "annealing/SimulatedAnnealing.hpp"
#include "io/IOFunctions.hpp"


int main(int argc, char* argv[])
{
	//Class that contains all the important matrices
	AllMatrices am;

	//Class that executes the algorithm
	SimulatedAnnealing sa;

	//Set values for matrices/vector. If provided, get number of iteration, G output
	//filename, and E output filename.
	CommandLineInfo cli = get_info(argc, argv, am);

	//Calculate T_G, T_E and the result of the T_G dot T_E. 
	am.setup();

	//Run the algorithm. Pass all the created matrices and vectors to the algorithm
	sa.run(std::move(am), cli.iterations);

	//Returns the result of the algorithm. The pair will be in the form of:
	//results.first = G
	//results.second = E
	std::pair<Matrix, Matrix> results = sa.SA_results();

	//Output G and E matrices to separate files.
	print_results(results, cli.G_output, cli.E_output);

	return 0;
}
