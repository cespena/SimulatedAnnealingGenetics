#include "AllMatrices.hpp"

AllMatrices& AllMatrices::operator=(AllMatrices&& am)
{
	*this = am;
	return *this;
}

//Assigns a matrix or vector its appropriate dimensions and values. When assigning
//GR_tr, do not assign values for r and c. The declaration of this function in 
//AllMatrices.hpp gives r and c a default value of 0. This is because r and c are
//not necessary when assigning GR_tr. Only the vector is important. 
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
		case 4:
			GR_tr = values;
			break;
		default:
			break;
	}
}

//Creates T_G, T_E and GR_guess_tr. Initializes sum values for the Pearson
//Correlation Coefficient.
void AllMatrices::setup()
{
	//Check that the matrices and vectors can do matrix multiplication with one another
	assert(G.get_cols() == X_tr.get_rows());
	assert(E.get_cols() == Y_tr.get_rows());

	//Check that T_G and T_E can perform dot product with one another
	assert(G.get_rows() == E.get_rows());
	assert(X_tr.get_cols() == Y_tr.get_cols());

	//Check that GR_tr and GR_guess_tr have the same dimensions
	assert( (int) GR_tr.size() == X_tr.get_cols());

	//Get T_G, T_E, and their dot product
	T_G = G * X_tr;
	T_E = E * Y_tr;

	//create GR_guess_tr vector
	GR_guess_tr.reserve(GR_tr.size());

	//Set initial values for GR_guess_tr
	for (int i = 0; i < T_G.get_cols(); i++)
	{
		double sum = 0;
		for (int j = 0; j < T_G.get_rows(); j++)	//Dot product of columns
			sum += T_G[j][i] * T_E[j][i];
		GR_guess_tr.push_back(sum); //Store dot product in vector
	}
	
	//Get the sums for the Pearson Correlation Coefficient
	sum_x = 0;
	sum_y = 0;
	sum_xy = 0;
	sum_xx = 0;
	sum_yy = 0;

	for (unsigned int i = 0; i < GR_tr.size(); i++)
	{
		sum_x += GR_tr[i];
		sum_xx += GR_tr[i] * GR_tr[i];
		sum_y += GR_guess_tr[i];
		sum_yy += GR_guess_tr[i] * GR_guess_tr[i];
		sum_xy += GR_tr[i] * GR_guess_tr[i];
	}

	//Initialize sum_of_difference_vector
	sum_of_difference_vector = 0;

}

//Used to reset Matrix G and E to original values. All other values
//that need to be reset will be reset. 
void AllMatrices::reset(Matrix& g, Matrix& e)
{
	G = g;
	E = e;
	GR_guess_tr.clear();
	setup();
}

//Changes the values in either matrix G or E, and updates the rest 
//of the variables that need updating due to the change in either 
//one of the two matrices. This change does recalculate everything
//all over again, but rather only updates the values that changes
//due to the change in matrix G or E.
double AllMatrices::change_value(int G_or_E, int r, int c, double new_value)
{

	Matrix* main_matrix;
	Matrix* xy_matrix;
	Matrix* first_T_matrix;
	Matrix* second_T_matrix;

	double old_matrix_value;

	//Used to set the pointer variables to the appropriate variables. This
	//depends on which of the two matrices is being changed.
	if (G_or_E == 0)
	{
		main_matrix = &G;
		xy_matrix = &X_tr;
		first_T_matrix = &T_G;
		second_T_matrix = &T_E;
	}
	else
	{
		main_matrix = &E;
		xy_matrix = &Y_tr;
		first_T_matrix = &T_E;
		second_T_matrix = &T_G;
	}

	//Change Matrix
	old_matrix_value = (*main_matrix)[r][c]; //Store old matrix value
	(*main_matrix)[r][c] = new_value;		//Change to new value

	//Reset sums involving GR_guess_tr for Pearson Correlation Coefficient calculation
	sum_y  = 0;
	sum_yy = 0;
	sum_xy = 0;

	//Reset sum for mean calculation
	sum_of_difference_vector = 0;

	for (int i = 0; i < first_T_matrix->get_cols(); i++)
	{
		//Update T_matrix
		double old_T_value = (*first_T_matrix)[r][i];	//Store old T_matrix value
		(*first_T_matrix)[r][i] += ( new_value - old_matrix_value ) * (*xy_matrix)[c][i]; //Update old value in T_matrix

		//Update GR_guess_tr
		GR_guess_tr[i] += ( (*first_T_matrix)[r][i] - old_T_value ) * (*second_T_matrix)[r][i]; //Update old value in GR_guess_tr
		
		//Update sum values that include GR_guess_tr
		sum_y  += GR_guess_tr[i];
		sum_yy += GR_guess_tr[i] * GR_guess_tr[i];
		sum_xy += GR_tr[i] * GR_guess_tr[i];

		//Add the absolute value of the individual elements of (GR_guess_tr - GR_tr)
		sum_of_difference_vector += std::abs(GR_guess_tr[i] - GR_tr[i]);

	}
	
	return old_matrix_value;

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

//Calculate Pearson Correlation Coefficient
double AllMatrices::pearson_correlation_coefficient()
{
	int n = GR_tr.size();
	double result = (n * sum_xy - sum_x * sum_y) / sqrt( (n * sum_xx - sum_x * sum_x) * (n * sum_yy - sum_y * sum_y) );
	return result;
}

//Calculate the mean of the elements of (GR_guess_tr - GR_tr)
double AllMatrices::difference_vector_mean()
{
	return sum_of_difference_vector / GR_tr.size();
}

//Returns G and E at the end of the algorithm
std::pair<Matrix, Matrix> AllMatrices::get_best()
{
	std::pair<Matrix, Matrix> pair_result = std::make_pair(G, E);
	return pair_result;
}

//For debugging. Helper function to print vectors
void AllMatrices::print_vector(std::string name, const std::vector<double>& vec)
{
	std::cout << name << ':' << std::endl;
	for (auto i : vec)
		std::cout << i << std::endl;
	std::cout << std::endl;
}

//Print all matrices. Used for debugging
void AllMatrices::print()
{
/*
	//Uncomment the matrices that you need to print
	std::cout << "G:\n" << G << std::endl;
	std::cout << "X_tr:\n" << X_tr << std::endl;
	std::cout << "T_G:\n" << T_G << std::endl;
	std::cout << "E:\n" << E << std::endl;
	std::cout << "Y_tr:\n" << Y_tr << std::endl;
	std::cout << "T_E:\n" << T_E << std::endl;
	print_vector("GR_guess_tr", GR_guess_tr);
	print_vector("GR_tr", GR_tr);

	std::cout << "sum_x:\t" << sum_x << std::endl;
	std::cout << "sum_xx:\t" << sum_xx << std::endl;
	std::cout << "sum_y:\t" << sum_y << std::endl;
	std::cout << "sum_yy:\t" << sum_yy << std::endl;
	std::cout << "sum_xy:\t" << sum_xy << std::endl;
	std::cout << "difference mean: " << difference_vector_mean() << std::endl;
*/

}
