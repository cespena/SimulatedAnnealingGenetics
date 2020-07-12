#ifndef SIMULATEDANNEALING_HPP
#define SIMULATEDANNEALING_HPP

#include <iostream>
#include <cmath>
#include <limits>
#include <random>
#include <utility>
#include <chrono>
#include "CircularBuffer.hpp"
#include "../matrix/AllMatrices.hpp"

#define SA_ITERATIONS 1e6

//Used for returning new random values
struct RandomNumbers
{

	int G_or_E;
	int row;
	int col;
	double new_value;
};

class SimulatedAnnealing
{
public:
	SimulatedAnnealing() = default;
	void run(AllMatrices&& matrices);
	std::pair<Matrix, Matrix> SA_results();

private:
	//Contains all matrices: G, E, X_tr, Y_tr, T_E, T_G and result of (T_E dot T_G)
	AllMatrices am;

	//Circular buffer to store bad values
	CircularBuffer cb;
	
	//Temperatures
	double current_temp;
	double initial_temp;
	double final_temp;

	//Iterations
	unsigned long int SA_iter;
	unsigned long int SA_numIters;
	double SA_lambda;

	//Variables for generating random numbers
	std::random_device device;
	std::default_random_engine engine{device()};
	std::uniform_int_distribution<int> matrix_selector{0, 1};
	double pBad;

	//Private Functions
	void Iter(double& T, double& threshold, double& obj, int& counter_bad, double& pbad_sum);
	double GetpBad(double& T, double& threshold);
	void set_temps();
	void set_iterations();
	RandomNumbers generate_random_numbers();
	RandomNumbers generate_random_numbers(double& threshold);
	double objective_function();



};

#endif //SIMULATEDANNEALING_HPP
