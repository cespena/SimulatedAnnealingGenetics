#include "AllMatrices.hpp"

AllMatrices& AllMatrices::operator=(AllMatrices&& am)
{
	*this = am;
	return *this;
}

//assigns a matrix or vector its appropriate dimensions and values
void AllMatrices::set_matrix(int selector, std::vector<double> values, int r, int c)
{
	switch(selector)
	{
		case 0:
			G = Matrix(values, r, c);
			break;
		case 1:
			E = Matrix(values, r, c);
			break;
		case 2:
			X_tr = Matrix(values, r, c);
			break;
		case 3:
			Y_tr = Matrix(values, r, c);
			break;
		default:
			break;
	}
}

//Creates T_G, T_E and the result of T_G dot T_E
void AllMatrices::setup()
{
	//Check that the matrices and vectors can do matrix multiplication with one another
	assert(G.get_cols() == X_tr.get_rows());
	assert(E.get_cols() == Y_tr.get_rows());

	//Check that T_G and T_E can perform dot product with one another
	assert(G.get_rows() == E.get_rows());
	assert(X_tr.get_cols() == Y_tr.get_cols());

	//Get T_G, T_E, and their dot product
	T_G = G * X_tr;
	T_E = E * Y_tr;
	result = T_G.vec_dot_product(T_E);

}

std::pair<double, double> AllMatrices::change_value(int G_or_E, int r, int c, double new_value)
{
	//Updates the values in matrix G or E, vectors T_G or T_E, and the dot
	//product of T_G and T_E. However, rather than doing dot products in 
	//order to recalulate everything, it is more efficient to see which 
	//elements need to be updated and by how much. This should greatly 
	//reduce time since the program won't need to loop through vectors
	//and matrices.
	Matrix* matrix_ptr;
	Matrix* xy_vector_ptr;
	Matrix* result_vector_ptr;
	Matrix* second_result_vector_ptr;
	double matrix_old_value;
	double result_vector_old_value;
	std::pair<double, double> old_values_pair;

	//Used to set the pointer variables to the appropriate variables. This
	//depends on which of the two matrices is being changed.
	if (G_or_E == 0)
	{
		matrix_ptr = &G;
		xy_vector_ptr = &X_tr;
		result_vector_ptr = &T_G;
		second_result_vector_ptr = &T_E;
	}
	else
	{
		matrix_ptr = &E;
		xy_vector_ptr = &Y_tr;
		result_vector_ptr = &T_E;
		second_result_vector_ptr = &T_G;
		
	}

	//retrieve old values from matrix and vector
	matrix_old_value = (*matrix_ptr)[r][c];
	result_vector_old_value = (*result_vector_ptr)[r][0];
	old_values_pair.first = matrix_old_value;
	old_values_pair.second = result;

	//replace entry in G or E matrix
	(*matrix_ptr)[r][c] = new_value;
	
	//replace entry in T_G or T_E vectors
	(*result_vector_ptr)[r][0] -= (matrix_old_value * (*xy_vector_ptr)[c][0]);
	(*result_vector_ptr)[r][0] += (new_value * (*xy_vector_ptr)[c][0]);
	
	//recalculate dot product result of T_G and T_E
	result -= (result_vector_old_value * (*second_result_vector_ptr)[r][0]);
	result += ((*result_vector_ptr)[r][0] * (*second_result_vector_ptr)[r][0]);

	
	return old_values_pair;

}

//Returns the dimensions of either matrix G or E
std::pair<int, int> AllMatrices::get_matrix_dimensions(int G_or_E)
{
	std::pair<int, int> result;
	
	if (G_or_E == 0)
	{
		result.first = G.get_rows();
		result.second = G.get_cols();
	}
	else
	{
		result.first = E.get_rows();
		result.second = E.get_cols();
	}
	return result;
}


//Returns result of T_G and T_E
double AllMatrices::get_result()
{
	return result;
}

//Returns G and E at the end of the algorithm
std::pair<Matrix, Matrix> AllMatrices::get_best()
{
	std::pair<Matrix, Matrix> pair_result = std::make_pair(G, E);
	return pair_result;
}

//Print all matrices. Used for debugging
void AllMatrices::print()
{
	//Uncomment the matrices that you need to print

	std::cout << "G:\n" << G << std::endl;
	std::cout << "E:\n" << E << std::endl;
/*
	std::cout << "X_tr:\n" << X_tr << std::endl;
	std::cout << "Y_tr:\n" << Y_tr << std::endl;
	std::cout << "T_G:\n" << T_G << std::endl;
	std::cout << "T_E:\n" << T_E << std::endl;
	std::cout << "result: " << result << std::endl;
*/
}
