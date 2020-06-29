#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>
#include <vector>
#include <utility>

//This stack implementation is used to keep track of the changes that 
//occur in the G and E matrices. In the algorithm, whenever we find
//a good improvement, G and E must be stored into G_best and E_best.
//With my previous implementation, it would copy both matrices entirely.
//This is a big waste of time since not all the values between the
//current and best matrices have changed. We should only update the 
//values that have changed between the two.
class Stack
{
public:
	Stack() = default;
	
	void setup(int matrix_size); //Stack setup
	void new_item(int row, int column, bool is_new); //Insert new item or remove old item
	bool empty(); //Return if stack empty or not
	std::pair<int, int> pop_stack(); //pop out and return element in stack
	void print(); //For debugging. Print the stack.


private:
	//The stack will be a vector of pairs. Each pair contains a 
	//row and column, which represents an element in the matrix.
	std::vector<std::pair<int, int>> stack; 

};

#endif //STACK_HPP
