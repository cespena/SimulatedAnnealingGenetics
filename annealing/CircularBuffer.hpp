#ifndef CIRCULARBUFFER_HPP
#define CIRCULARBUFFER_HPP

#include <vector>
#include <iostream>

//Circular Buffer used for bad values. Note that this circular buffer
//uses a 0 based array as opposed to a 1 based array that R vectors use.
class CircularBuffer
{
public:
	CircularBuffer();
	void insert_value(double value);
	double get_pBad();
	void print();

private:
	int num_circ;
	std::vector<double> circ_buf;
	double sum_circ;
	int i_circ;

};

#endif //CIRCULARBUFFER_HPP
