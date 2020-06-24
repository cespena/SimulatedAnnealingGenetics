#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>
#include <utility>

class Matrix
{
public:
	//Constructors
	Matrix() = default;
	Matrix(const Matrix&) = default;
	Matrix(const int& r, const int& c);
	Matrix(const std::vector<double>& values, const int& r, const int& c);
	Matrix& operator=(Matrix& m);
	Matrix& operator=(Matrix&& m);

	//Accessing member variables	
	std::vector<double>& operator[](int index);	//To access elements: Matrix[row][col]
	int get_rows() const;
	int get_cols() const;

	//Math Operations
	Matrix operator*(Matrix& rhs) const;		//Matrix Multiplication
	double vec_dot_product(Matrix& rhs) const;	//Dot product between vectors (Matrices with only 1 column)

	//Output Matrix
	friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

private:
	int rows;
	int cols;
	std::vector<std::vector<double>> matrix;

	//Used for matrix multiplication
	double dot_product(const std::vector<double>& lhs_row, const int& rhs_col, Matrix& rhs) const;

};

#endif //MATRIX_HPP
