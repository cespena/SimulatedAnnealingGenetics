#include <iostream>

#include "matrix/Matrix.hpp"
#include "matrix/AllMatrices.hpp"
#include "annealing/SimulatedAnnealing.hpp"

int main()
{
	//Class that contains all the important matrices
	AllMatrices am;

	//Class that executes the algorithm
	SimulatedAnnealing sa;


	//Used to create initial matrices and vectors. Doing this temporarily since
	//I am not sure how the matrices and vectors will be given. I am currently 
	//using the sizes of the matrices and vectors from the original program.
	std::vector<std::pair<int, int>> new_matrices;
	new_matrices.push_back(std::make_pair(2,  291));	//G dimensions
	new_matrices.push_back(std::make_pair(2,   18));	//E dimensions
	new_matrices.push_back(std::make_pair(291, 62));	//X_tr dimensions
	new_matrices.push_back(std::make_pair(18,  62));	//Y_tr dimensions
	new_matrices.push_back(std::make_pair(62,   1));	//GR_tr dimensions


	//Randomly generate values for matrices and vectors
	std::random_device device;
	std::default_random_engine engine{device()};
	std::uniform_real_distribution<double> distribution{-3, 3};

	for (int i = 0; i < 5; i++)
	{
		int max = new_matrices[i].first * new_matrices[i].second;
		std::vector<double> temp;
		temp.reserve(max);
		for (int j = 0; j < max; j++)
			temp.push_back(distribution(engine));
		am.set_matrix(i, temp, new_matrices[i].first, new_matrices[i].second);
	}

	//Calculate T_G, T_E and the result of the T_G dot T_E. 
	am.setup();

	//Run the algorithm. Pass all the created matrices and vectors to the algorithm
	sa.run(std::move(am));

	//Returns the result of the algorithm. The pair will be in the form of:
	//results.first = G
	//results.second = E
	std::pair<Matrix, Matrix> results = sa.SA_results();

	//For debugging. Use these lines to print the resulting G and E vectors
	/*
	std::cout << "The results are: " << std::endl;
	std::cout << "G:\n" << results.first << std::endl;
	std::cout << "E:\n" << results.second << std::endl;
	*/

	return 0;
}
