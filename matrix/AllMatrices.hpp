#ifndef ALLMATRICES_HPP
#define ALLMATRICES_HPP

#include <cassert>
#include <utility>
#include <functional>
#include <unordered_map>
#include "Matrix.hpp"

//In order for the HashMap to use std::pair as its Key, a custom hash function
//needs to be implemented.
struct pair_hash
{
	std::size_t operator() (const int& element) const
	{
		return element;
	}
};

class AllMatrices
{
public:
	AllMatrices() = default;
	AllMatrices(const AllMatrices&) = default;
	AllMatrices& operator=(AllMatrices&) = default;
	AllMatrices& operator=(AllMatrices&& am);
	
	//Setup functions
	void set_matrix(int selector, std::vector<double> values, int r, int c);	//Assign matrix/vector its dimensions and values
	void setup();	//Create T_G, T_E and T_G dot T_E

	//Used to give random value to a random matrix's element
	std::pair<double, double> change_value(int G_or_E, int r, int c, double value);

	//Accessing 
	std::pair<int, int> get_matrix_dimensions(int G_or_E);	//get dimensions of either G or E
	double get_result();	//Returns T_G dot T_E
	
	//Handle best matrices
	void update_best();	//update G_best and E_best to the current G and E matrices
	std::pair<Matrix, Matrix> get_best();	//return G_best and E_best
	
	//For debugging. Prints all the member variables.
	void print();



private:
	//HashMaps that will keep track of the changes of G and E. Uses rows, and columns
	//as the Key and the new change as the Value. The Keys will be in the form of an 
	//std::pair(row, col). Since we are using an std::pair, the current hash function
	//will not work. A custom hash function needs to be implemented.
	std::unordered_map<int, double, pair_hash> G_changes;
	std::unordered_map<int, double, pair_hash> E_changes;

	Matrix G;
	Matrix E;
	Matrix G_best;
	Matrix E_best;
	Matrix X_tr;
	Matrix Y_tr;
	Matrix T_G;
	Matrix T_E;
	double result;

	//Private Functions

	int hash_element(int r, int c, int columns); //Hash function to hash rows and columns
	std::pair<int, int> unhash_element(int element, int columns); //Unhash functions to return key to rows and columns


};

#endif //ALLMATRICES_HPP
