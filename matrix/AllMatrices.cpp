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

	//Set G_best and E_best to the current initial values of G and E respectively
	G_best = G;
	E_best = E;

	//Reserve a decent bucket count in order to avoid rehashing of the HashMap. Currently, 
	//the bucket count will be the size of their respective matrices.
	int G_size = G.get_rows() * G.get_cols();
	int E_size = E.get_rows() * E.get_cols();

	G_changes.reserve(G_size);
	E_changes.reserve(E_size);

	for (int i = 0; i < G_size; i++)
		G_changes.push_back(0);

	for (int i = 0; i < E_size; i++)
		E_changes.push_back(0);

	//Change the load factor so that the HashMaps won't rehash themselves
	//G_changes.max_load_factor(4.0);
	//E_changes.max_load_factor(4.0);

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
		int hashed_value = hash_element(r, c, G.get_cols());
		G_changes[hashed_value] = new_value; //Include change into G_changes
		G_queue.push_back(hashed_value);
	}
	else
	{
		matrix_ptr = &E;
		xy_vector_ptr = &Y_tr;
		result_vector_ptr = &T_E;
		second_result_vector_ptr = &T_G;
		int hashed_value = hash_element(r, c, E.get_cols());
		E_changes[hashed_value] = new_value; //Include change into E_changes
		E_queue.push_back(hashed_value);
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

	//print(); //remember to remove//////////////////
	
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

//Updates current G_best and E_best to current G and E
void AllMatrices::update_best()
{
	//G_best = G;
	//E_best = E;
	//Before, G_best and E_best would cope every element of G and E 
	//respectively. This is a waste of time since not all the elements
	//change when updating G_best and E_best. Some elements may remain 
	//the same. Now, a HashMap is used so that only the changes are
	//recorded. This means that copying should be much more efficient
	//since only the changes between the matrices are being recorded.

	//Make the changes to G_best
	/*for(auto change = G_changes.begin(); change != G_changes.end(); change++)
	{
		//G_best[change->first.first][change->first.second] = change->second;
		std::pair<int, int> rows_cols = unhash_element(change->first, G.get_cols());
		G_best[rows_cols.first][rows_cols.second] = change->second;
	}*/

	while (!G_queue.empty())
	{
		std::pair<int, int> rows_cols = unhash_element(G_queue.back(), G.get_cols());
		G_best[rows_cols.first][rows_cols.second] = G_changes[G_queue.back()];
		G_queue.pop_back();
	}

	//Make the changes to E_best
	/*for (auto change = E_changes.begin(); change != E_changes.end(); change++)
	{
		//E_best[change->first.first][change->first.second] = change->second;
		std::pair<int, int> rows_cols = unhash_element(change->first, E.get_cols());
		E_best[rows_cols.first][rows_cols.second] = change->second;
	}*/

	while (!E_queue.empty())
	{
		std::pair<int, int> rows_cols = unhash_element(E_queue.back(), E.get_cols());
		E_best[rows_cols.first][rows_cols.second] = E_changes[E_queue.back()];
		E_queue.pop_back();
	}

	//Since the best matrices have the same values as their respective matrices, 
	//their HashMaps can be cleared so that the next time the best matrices need 
	//to be updated, we don't need to loop through old values.
	//G_changes.clear();
	//E_changes.clear();
}

//Returns G_best and E_best at the end of the algorithm
std::pair<Matrix, Matrix> AllMatrices::get_best()
{
	std::pair<Matrix, Matrix> result = std::make_pair(G_best, E_best);
	return result;
}

//Print all matrices. Used for debugging
void AllMatrices::print()
{
	//Uncomment the matrices that you need to print
/*
	std::cout << "G:\n" << G << std::endl;
	std::cout << "E:\n" << E << std::endl;
	std::cout << "G_best\n" << G_best << std::endl;
	std::cout << "E_best\n" << E_best << std::endl;
	std::cout << "X_tr:\n" << X_tr << std::endl;
	std::cout << "Y_tr:\n" << Y_tr << std::endl;
	std::cout << "T_G:\n" << T_G << std::endl;
	std::cout << "T_E:\n" << T_E << std::endl;
	std::cout << "result: " << result << std::endl;
*/
}


////Private Functions

int AllMatrices::hash_element(int r, int c, int columns)
{
	return (r * columns) + c;
}

std::pair<int, int> AllMatrices::unhash_element(int element, int columns)
{
	return std::make_pair(element / columns, element % columns);
}
