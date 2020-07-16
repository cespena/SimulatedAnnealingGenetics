#include "SimulatedAnnealing.hpp"

void SimulatedAnnealing::run(AllMatrices&& matrices, unsigned long int iters)
{
	//get all the matrices that were created;
	am = matrices;

	//For debugging
	int if_count = 0;
	int else_count = 0;

	//Initialize temperatures and loop counters
	set_temps();
	set_iterations(iters);

	//For debugging. Print initial and final temps.
	//std::cout << "Initial temp: " << initial_temp << "\tFinal temp: " << final_temp << std::endl;

	//Probability of accepting bad result
	pBad = 0.1;
	//Store best (lowest) objective value so far
	double obj = 1e30;

	std::uniform_real_distribution<double> distribution(0.0, std::nextafter(1.0, std::numeric_limits<double>::max()));

	//Used for timing loop.
	auto start = std::chrono::high_resolution_clock::now();

	while(SA_iter < SA_numIters)
	{
		//Choose randomly between Matrix G or E and select an element to random change its value
		RandomNumbers rn = generate_random_numbers();

		//Update the appropriate matrix. Store the old value that was changed
		double old_value = am.change_value(rn.G_or_E, rn.row, rn.col, rn.new_value);


		//Get result of objective function
		double obj_new = objective_function();


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
			obj = obj_new;
		}
		else	//if not, revert back to old values and insert pAcc to circular buffer
		{
			else_count++;	//for debugging
			am.change_value(rn.G_or_E, rn.row, rn.col, old_value);
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
	//std::cout << "Time taken: " << duration.count() << std::endl;

	//For debugging. Used to count accepted and rejected
	//std::cout << "count is: " << SA_iter << '\t' << "if's: " << if_count  << '\t' << "else's: " << else_count << std::endl;
}

//Returns G and E. Call this after the algorithm has executed
std::pair<Matrix, Matrix> SimulatedAnnealing::SA_results()
{
	return am.get_best();
}

////Private Functions////

//Used to calculate initial/final temps. Does a single iteration 
//at a specified temperature.
void SimulatedAnnealing::Iter(double& T, double& threshold, double& obj, int& counter_bad, double& pbad_sum)
{
	//generate random numbers
	RandomNumbers rn = generate_random_numbers(threshold);

	//Update the appropriate matrix. Store the old value that was changed.
	double old_value = am.change_value(rn.G_or_E, rn.row, rn.col, rn.new_value);

	//Get result of objective function
	double obj_new = objective_function();

	//Calculate the possibility of accuracy, and compate it with a random
	//number between 0 and 1 in order to compare
	std::uniform_real_distribution<double> distribution(0.0, std::nextafter(1.0, std::numeric_limits<double>::max()));
	double SA_pAcc = exp( -(obj_new - obj) / T);
	double SA_r = distribution(engine);

	if (SA_r < SA_pAcc)
	{
		obj = obj_new;
	}
	else
	{
		am.change_value(rn.G_or_E, rn.row, rn.col, old_value);
		counter_bad += 1;
		pbad_sum += SA_pAcc;
	}
}

//Used to calculate initial/final temps. Gets the average pBad 
//at a given temperature by calling Iter function a large
//amount of times.
double SimulatedAnnealing::GetpBad(double& T, double& threshold)
{
	int iter_counter = 0;
	int counter_bad  = 0;
	double pbad_sum  = 0;
	double obj    = 1e30;
	
	while (iter_counter < 10000)
	{
		Iter(T, threshold, obj, counter_bad, pbad_sum);
		iter_counter++;
	}
	
	if (counter_bad == 0)
		return 1;

	return (pbad_sum / counter_bad);
}

//Used to set temperature variables
void SimulatedAnnealing::set_temps()
{
	//For debugging. Counters for while loops
	int w1 = 0;
	int w2 = 0;
	int w3 = 0;
	
 	//Store the original G and E matrices. Need to revert back
 	//to the original G and E matrices every time GetpBad is 
 	//called.
 	std::pair<Matrix, Matrix> originals = am.get_best();

	double T = 1;					//Start with 1
	double targetpBad = 0.99;		//Initially, 99% chance of accepting bad moves
	
	//Find starting temperature
	while (GetpBad(T, targetpBad) > targetpBad)	//May be too high already
	{
		T /= 10;
		am.reset(originals.first, originals.second);
		w1++;
	}

	while (GetpBad(T, targetpBad) < targetpBad)	//Jump back up by factors of 2
	{
		T *= 2;
		am.reset(originals.first, originals.second);
		w2++;
	}

	initial_temp = T;				//Set initial temp

	//Find ending temperature
	targetpBad = 1e-6;				//want to end with tiny pBad
	
	while (GetpBad(T, targetpBad) > targetpBad)	//Jump down by factors of 10 until pBad < 1e-6
	{
		T /= 10;
		am.reset(originals.first, originals.second);
		w3++;
	}

	final_temp = T;					//Set final temp
	
	am.reset(originals.first, originals.second);

	//For debugging. Print out the counters of each while loop.
	//std::cout << "while1: " << w1 << "\twhile2: " << w2 << "\twhile3: " << w3 << std::endl;

	//Set the lambda constant
	SA_lambda = -log(final_temp / initial_temp);
}

//Used to set while loop iterations
void SimulatedAnnealing::set_iterations(unsigned long int iters)
{
	//Set inital iteration and final iteration 
	//Change the value of SA_ITERATIONS in SimulatedAnnealing.hpp
	SA_iter = 0;
	SA_numIters = iters; 
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

//Used to generate a random value for a randomly selected matrix. This function is
//different from the one above only on how they calculate the random number. This
//function is used when finding the initial and final temperatures.
RandomNumbers SimulatedAnnealing::generate_random_numbers(double& threshold)
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
	std::uniform_real_distribution<double> new_value_generator{ -sqrt(threshold), sqrt(threshold) };
	result.new_value = new_value_generator(engine);

	return result;
}


//Returns the result of the objective function
//Updated: Uses the proper objective function calculation. Most of the calculations
//are done in AllMatrices.cpp. This function is more to see the overview of the 
//calculation of the objective function.
double SimulatedAnnealing::objective_function()
{
	double rho = am.pearson_correlation_coefficient();
	double GR_mean_diff = am.difference_vector_mean();
	return std::abs( GR_mean_diff * (1 - rho)  );
}

