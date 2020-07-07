#ifndef ALLMATRICES_HPP
#define ALLMATRICES_HPP

#include <cassert>
#include <cmath>
#include <utility>
#include "Matrix.hpp"

class AllMatrices
{
public:
	AllMatrices() = default;
	AllMatrices(const AllMatrices&) = default;
	AllMatrices& operator=(AllMatrices&) = default;
	AllMatrices& operator=(AllMatrices&& am);
	
	//Setup functions
	void set_matrix(int selector, std::vector<double> values, int r, int c);	//Assign matrix/vector its dimensions and values
	void setup();	//Create T_G, T_E and GR_guess_tr.

	//Used to give random value to a random matrix's element
	//Updated to accept bool value
	double change_value(int G_or_E, int r, int c, double value);

	//Accessing 
	std::pair<int, int> get_matrix_dimensions(int G_or_E);	//get dimensions of either G or E
	double pearson_correlation_coefficient();
	double difference_vector_mean();
	
	//Handle best matrices
	std::pair<Matrix, Matrix> get_best();	//return G and E
	
	//For debugging. Prints all the member variables.
	void print();


private:
	//Given Matrices
	Matrix G;
	Matrix E;
	Matrix X_tr;
	Matrix Y_tr;
	Matrix GR_tr;

	//Need to be made
	Matrix T_G;
	Matrix T_E;
	Matrix GR_guess_tr;

	//Sum values to calculate Pearson Correlation Coefficient
	double sum_x;
	double sum_y;
	double sum_xy;
	double sum_xx;
	double sum_yy;

	//Value to calculate average difference between the simulated. This 
	//is to hold the sum of elements of (GR_guess_tr - GR_tr). Before adding
	//the values to the sum, the absolute value must be taken of each element.
	double sum_of_difference_vector;


};

#endif //ALLMATRICES_HPP
