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
/*
	//Used to create initial matrices and vectors. Doing this temporarily since
	//I am not sure how the matrices and vectors will be given. I am currently 
	//using the sizes of the matrices and vectors from the original program.
	std::vector<std::pair<int, int>> new_matrices;
	new_matrices.push_back(std::make_pair(900, 1000));	//G dimensions
	new_matrices.push_back(std::make_pair(900, 700));	//E dimensions
	new_matrices.push_back(std::make_pair(1000, 1));	//X_tr dimensions
	new_matrices.push_back(std::make_pair(700,  1));	//Y_tr dimensions


	//Randomly generate values for matrices and vectors
	std::random_device device;
	std::default_random_engine engine{device()};
	std::uniform_real_distribution<double> distribution{-3, 3};

	for (int i = 0; i < 4; i++)
	{
		int max = new_matrices[i].first * new_matrices[i].second;
		std::vector<double> temp;
		temp.reserve(max);
		for (int j = 0; j < max; j++)
			temp.push_back(distribution(engine));
		am.set_matrix(i, temp, new_matrices[i].first, new_matrices[i].second);
	}
*/

	std::vector<double> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	std::vector<double> v2 = {1, 2, 3, 4};
	std::vector<double> v3 = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	std::vector<double> v4 = {4, 3, 2, 1};

	std::vector<std::pair<int, int>> new_matrices;
	new_matrices.push_back(std::make_pair(4, 4));	//G dimensions
	new_matrices.push_back(std::make_pair(4, 4));	//E dimensions
	new_matrices.push_back(std::make_pair(4, 1));	//X_tr dimensions
	new_matrices.push_back(std::make_pair(4,  1));	//Y_tr dimensions

	am.set_matrix(0, v1, new_matrices[0].first, new_matrices[0].second);
	am.set_matrix(1, v3, new_matrices[1].first, new_matrices[1].second);
	am.set_matrix(2, v2, new_matrices[2].first, new_matrices[2].second);
	am.set_matrix(3, v4, new_matrices[3].first, new_matrices[3].second);

	//Calculate T_G, T_E and the result of the T_G dot T_E. 
	//Updated: Set G_best and E_best to the to the current 
	//initial values of G and E respectively. Setup the G_stack 
	//and E_stack.
	am.setup();

	//Run the algorithm. Pass all the created matrices and vectors to the algorithm
	sa.run(std::move(am));

	//Results.first = G_best
	//Results.second = E_best
	//Currently, only one value out of the two matrices is being changed since the
	//initial matrices were given before the algorithm began. I am hoping it is 
	//because there is no objective function yet, rather than it being a bug in 
	//my program
	std::pair<Matrix, Matrix> results = sa.SA_results();

	//Use these lines to print the resulting G_best and E_best vectors
	/*
	std::cout << "The results are: " << std::endl;
	std::cout << "G:\n" << results.first << std::endl;
	std::cout << "E:\n" << results.second << std::endl;
	*/

	return 0;
}
