#include "Matrix.hpp"

//Constructors
Matrix::Matrix(const int& r, const int& c)
{
	matrix.reserve(r);
	for (int i = 0; i < r; i++)
	{
		std::vector<double> temp;
		temp.reserve(c);
		for (int j = 0; j < c; j++)
			temp.push_back(0);
		matrix.push_back(temp);
	}
	rows = r;
	cols = c;
}

Matrix::Matrix(const std::vector<double>& values, const int& r, const int& c)
{
	matrix.reserve(r);

	int values_count = 0;
	for (int i = 0; i < r; i++)
	{
		std::vector<double> temp;
		temp.reserve(c);
		for (int j = 0; j < c; j++)
		{
			temp.push_back(values[values_count++]);
		}
		matrix.push_back(temp);
	}

	rows = r;
	cols = c;
}

Matrix& Matrix::operator=(Matrix& m)
{
	std::vector<std::vector<double>> new_matrix;
	if (this != &m)
	{
		new_matrix.reserve(m.get_rows());
		for (int r = 0; r < m.get_rows(); r++)
		{
			std::vector<double> temp;
			temp.reserve(m.get_cols());
			for (int c = 0; c < m.get_cols(); c++)
			{
				temp.push_back(m[r][c]);
			}
			new_matrix.push_back(temp);
		}
		matrix = new_matrix;
		rows = m.get_rows();
		cols = m.get_cols();
	}

	return *this;
}

Matrix& Matrix::operator=(Matrix&& m)
{
	matrix = std::move(m.matrix);
	rows = std::move(m.rows);
	cols = std::move(m.cols);
	return *this;
}

//Accessing member variables
std::vector<double>& Matrix::operator[](int index)
{
	return matrix.at(index);
}

int Matrix::get_rows() const
{
	return rows;
}

int Matrix::get_cols() const
{
	return cols;
}

//Math Operations
Matrix Matrix::operator*(Matrix& rhs) const
{
	Matrix result(rows, rhs.get_cols());

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < rhs.get_cols(); c++)
		{
			result[r][c] = dot_product(matrix[r], c, rhs);
		}
	}
	return result;
}

double Matrix::vec_dot_product(Matrix& rhs) const
{
	double result = 0;
	for (int i = 0; i < rows; i++)
		result += matrix[i][0] * rhs[i][0];
	return result;
}

//Output Matrix
std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
	for (auto i : m.matrix)
	{
		for (auto j : i)
			os << j << '\t';
		os << '\n';
	}
	return os;
}


//Private Member Functions
double Matrix::dot_product(const std::vector<double>& lhs_row, const int& rhs_col, Matrix& rhs) const
{
	double result = 0;
	for (int i = 0; i < rows; i++)
		result += lhs_row[i] * rhs[i][rhs_col];
	return result;
}

