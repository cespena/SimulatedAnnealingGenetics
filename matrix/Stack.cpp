#include "Stack.hpp"

//Setup the stack
void Stack::setup(int matrix_size)
{
	//Reserve a decent size into the stack so that it does not need
	//to do reallocation. Cuurently, the capacity of the stack will 
	//be 1/2 the size of its corresponding matrix.
	stack.reserve(matrix_size / 2);
}

//Insert new element or remove previous element. This is used to avoid
//storing any elements in the matrix that were reverted back to their 
//old values.
void Stack::new_item(int row, int column, bool is_new)
{
	if (is_new)	//If the value is new, insert into stack
		stack.push_back(std::make_pair(row, column));
	else 		//If not, remove the previous value
		stack.pop_back();
}

//Return bool value to determine if stack is empty
bool Stack::empty()
{
	return stack.empty();
}

//Remove and return the last element in the stack.
std::pair<int, int> Stack::pop_stack()
{
	std::pair<int, int> result = std::make_pair(stack.back().first, stack.back().second);
	stack.pop_back();
	return result;
}

//For debugging. Print the stack
void Stack::print()
{
	for (auto i : stack)
		std::cout << '(' << i.first << ", " << i.second << ") ";
	std::cout << std::endl;
}
