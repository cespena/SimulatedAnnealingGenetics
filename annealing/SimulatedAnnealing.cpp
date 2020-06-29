#include "SimulatedAnnealing.hpp"

void SimulatedAnnealing::run(AllMatrices&& matrices)
{
	//get all the matrices that were created;
	am = matrices;

	//For debugging
	int if_count = 0;
	int if_if_count = 0;
	int else_count = 0;

	//Initialize temperatures and loop counters
	set_temps();
	set_iterations();
	
	//Probability of accepting bad result
	pBad = 0.1;
	//Store best (lowest) objective value so far
	double obj = 1e30;

	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	//Used for timing loop.
	auto start = std::chrono::high_resolution_clock::now();

	while(SA_iter < SA_numIters)
	{
		//Choose randomly between Matrix G or E and select an element to random change its value
		RandomNumbers rn = generate_random_numbers();

		//Update the appropriate matrix. 
		//old_values.first = value of old element that was changed.
		//old_values.second = value of old dot product of vectors T_G and T_E
		std::pair<double, double> old_values = am.change_value(rn.G_or_E, rn.row, rn.col, rn.new_value, true);


		//Get result of objective function
		double obj_new = objective_function(am.get_result());


		//increase iterations
		double SA_s = (double) (SA_iter++) / SA_numIters;
		current_temp = initial_temp * exp(-SA_lambda * SA_s);

		//Calculate the possibility of accuracy. Generate a random number between 0 <= SA_r < 1
		//in order to compare
		double SA_pAcc = exp( -(obj_new - obj) / current_temp);
		double SA_r = distribution(engine);

		if (SA_r < SA_pAcc)	//If there is improvement, save appropriate variables
		{
			if_count++;	//for debugging
			if(obj > obj_new)
			{
				if_if_count++;	//for debuggin
				am.update_best();
			}

			obj = obj_new;
		}
		else	//if not, revert back to old values and insert pAcc to circular buffer
		{
			else_count++;	//for debugging
			am.change_value(rn.G_or_E, rn.row, rn.col, old_values.first, false);
			cb.insert_value(SA_pAcc);
		}
		
		//Update p_bad by getting the average of the circular buffer
		pBad = cb.get_pBad();

		//For debugging. Output iteration of main loop
		//if (SA_iter % 10000 == 0)
		//	std::cout << "Iteration #" << SA_iter  << "\t\ttemp: " << current_temp << std::endl;
		
	}

	//Used for timing loop.
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds> (stop - start);
	std::cout << "Time taken: " << duration.count() << std::endl;

	//For debugging. Used to count accepted and rejected
	std::cout << "count is: " << SA_iter << '\t' << "if's: " << if_count << '\t' << "if^2: " << if_if_count << '\t' << "else's: " << else_count << std::endl;
}

//Returns G_best and E_best. Call this after the algorithm has executed
std::pair<Matrix, Matrix> SimulatedAnnealing::SA_results()
{
	return am.get_best();
}

////Private Functions////

//Used to set temperature variables
void SimulatedAnnealing::set_temps()
{
	//Get inital and final temperatures. Set current temp to inital temp
	initial_temp = 100.0;
	final_temp = 1.0;
	current_temp = initial_temp;

	//Set the lambda constant
	SA_lambda = -log(final_temp / initial_temp);
}

//Used to set while loop iterations
void SimulatedAnnealing::set_iterations()
{
	//Set inital iteration and final iteration 
	//Using 1,000,000 for final iteration in order to test
	//how many iterations happen per second
	SA_iter = 0;
	SA_numIters = 10e5;
}

//Used to generae a random value for a randomly selected matrix
RandomNumbers SimulatedAnnealing::generate_random_numbers()
{
	RandomNumbers result;

	//Randomly select matrix
	result.G_or_E = matrix_selector(engine);
	std::pair<int, int> matrix_dimensions = am.get_matrix_dimensions(result.G_or_E);

	//Randomly select row and column
	std::uniform_int_distribution<int> row_selector{0, matrix_dimensions.first - 1};
	std::uniform_int_distribution<int> col_selector{0, matrix_dimensions.second - 1};
	result.row = row_selector(engine);
	result.col = col_selector(engine);

	//Randomly generate new value
	if (pBad > 1e-11)
	{
		std::uniform_real_distribution<double> new_value_generator{ -sqrt(pBad), sqrt(pBad) };
		result.new_value = new_value_generator(engine);
	}
	return result;
}

//Returns the result of the objective function
//Updated: Included the value of (T_G dot T_E) as a parameter. Rather than returning 
//0.5 all the time, (T_G dot T_E) * 0.5 shall be used. This is still temporary as a
//real objective function has not be given, however this temporary implementation 
//will make the algorithm work.
double SimulatedAnnealing::objective_function(double result)
{
	//Until an objective function is found, result * 0.5 will be returned
	return result * 0.5;
}

