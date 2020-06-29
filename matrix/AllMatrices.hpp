#ifndef ALLMATRICES_HPP
#define ALLMATRICES_HPP

#include <cassert>
#include <utility>
#include "Matrix.hpp"
#include "Stack.hpp"

class AllMatrices
{
public:
	AllMatrices() = default;
	AllMatrices(const AllMatrices&) = default;
	AllMatrices& operator=(AllMatrices&) = default;
	AllMatrices& operator=(AllMatrices&& am);
	
	//Setup functions
	void set_matrix(int selector, std::vector<double> values, int r, int c);	//Assign matrix/vector its dimensions and values
	void setup();	//Create T_G, T_E and T_G dot T_E. Updated: Set best matrices to equal the current matrices. Set up stacks

	//Used to give random value to a random matrix's element
	//Updated to accept bool value
	std::pair<double, double> change_value(int G_or_E, int r, int c, double value, bool is_new);

	//Accessing 
	std::pair<int, int> get_matrix_dimensions(int G_or_E);	//get dimensions of either G or E
	double get_result();	//Returns T_G dot T_E
	
	//Handle best matrices
	void update_best();	//update G_best and E_best to the current G and E matrices. Updated: Use stacks to update best matrices
	std::pair<Matrix, Matrix> get_best();	//return G_best and E_best
	
	//For debugging. Prints all the member variables.
	void print_stacks();
	void print();

private:
	//Stacks that will keep track of the changes of G and E. Push (row, column) pairs
	//onto the stack. When we need to update, we pop out the coordinates and search
	//for the new value inside of the current matrices.
	Stack G_stack;
	Stack E_stack;

	Matrix G;
	Matrix E;
	Matrix G_best;
	Matrix E_best;
	Matrix X_tr;
	Matrix Y_tr;
	Matrix T_G;
	Matrix T_E;
	double result;

};

#endif //ALLMATRICES_HPP
