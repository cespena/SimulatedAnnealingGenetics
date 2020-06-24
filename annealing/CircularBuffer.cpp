#include "CircularBuffer.hpp"

//Circular buffer of size 1000. In C++, we use 0 based arrays as 
//opposed to R which uses 1 based arrays
CircularBuffer::CircularBuffer()
{
	num_circ = 1000; //Change 1000 for different size circular buffer
	circ_buf.reserve(num_circ);
	for (int i = 0; i < num_circ; i++)
		circ_buf.push_back(1.0);
	sum_circ = num_circ;
	i_circ = 0;
}

//Insert value into the buffer
void CircularBuffer::insert_value(double value)
{
	//Once i_circ reaches the end of the vector, i_circ will go back 
	//to element 0 of the vector.
	i_circ++;
	//i_circ should be between [0, 1000)
	if (i_circ >= num_circ)
		i_circ = 0;
	
	sum_circ -= circ_buf[i_circ];
	circ_buf[i_circ] = value;
	sum_circ += value;
}

//Returns the average of all values stored in circular buffer
double CircularBuffer::get_pBad()
{
	return sum_circ / num_circ;
}

//Print buffer. Used for debugging
void CircularBuffer::print()
{
	for (auto i : circ_buf)
		std::cout << i << '\t';
	std::cout << std::endl;
}
