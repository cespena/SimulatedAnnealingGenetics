#include "IOFunctions.hpp"

//Prints usage manual for program.
void print_usage()
{
	std::cout << "Usage: Train_SA [OPTION FILE]..." << std::endl;
	std::cout << std::endl;
	std::cout << "  -G f\t\tGet G matrix from file f" << std::endl;
	std::cout << "  -g f\t\tGet GR_tr vector from file f" << std::endl;
	std::cout << "  -E f\t\tGet E matrix from file f" << std::endl;
	std::cout << "  -X f\t\tGet X_tr matrix from file f" << std::endl;
	std::cout << "  -Y f\t\tGet Y_tr matrix from file f" << std::endl;
	std::cout << "  -OG f\t\tSet output file f for G matrix" << std::endl;
	std::cout << "  -OE f\t\tSet output file f for E matrix" << std::endl;
	std::cout << "  -N n\t\tSet number of iterations of the program." << std::endl;
	std::cout << std::endl;
	std::cout << "In order for the program to work properly, all 5 flags for the matrices/vectors " << std::endl;
	std::cout << "(G, g, E, X, Y) must be set. All other flags (OG, OE, N) are optional. They can " << std::endl;
	std::cout << "be placed in any order." << std::endl;
	std::cout << std::endl;
	std::cout << "Number of iterations and output files can be optionally set. By default, number " << std::endl;
	std::cout << "of iterations is 1,000,000 and output files for G and E matrices are G_results " << std::endl;
	std::cout << "and E_results respectively." << std::endl;
	std::cout << std::endl;
	std::cout << "The files must be in .tsv (tab-separated) format with the exclusion of the " << std::endl;
	std::cout << "header. Each row belongs on a separate line. Each column value in a line must " << std::endl;
	std::cout << "be separated by a tab." << std::endl;

}

//Prints error messages
void print_error(int error_num, std::string extra_string)
{
	std::cerr << "Error: ";
	switch (error_num)
	{
		case 1:
			std::cerr << "Incorrect number of arguments. 10 arguments required. Provided " << extra_string << std::endl;
			break;
		case 2:
			std::cerr << extra_string << " is not a valid flag." << std::endl;
			break;
		case 3:
			std::cerr << "File '" << extra_string << "' cannot be opened" << std::endl;
			break;
		case 4:
			std::cerr << extra_string << " is not an integer." << std::endl;
			break;
	}
	std::cerr << "Try 'Train_SA -h' for help. Ending program..." << std::endl;

	exit(error_num);
}

//Gets values from file and assigns it to the appropriate matrix/vector.
void get_file_values(char flag, std::string filename, AllMatrices& am)
{
	std::string result = am.set_values(flag, filename);
	
	if (result != "")
		print_error(2, result);
}

//Checks whether provided file exists
bool is_file(std::string file)
{
	std::ifstream f(file);

	if (!f.is_open())
		print_error(3, file);

	return f.is_open();
}

//Gets the number of iterations.
int get_iterations(std::string iter_str)
{
	int result;

	try
	{
		result = std::stoi(iter_str);
	}
	catch (const std::invalid_argument& ia)
	{
		print_error(4, iter_str);
	}

	return result;
}

//Gets command line arguments given by user.
CommandLineInfo get_info(int argc, char* argv[], AllMatrices& am)
{
	struct CommandLineInfo cli;

	if (argc == 2 && !strcmp(argv[1], "-h"))
	{
		print_usage();
		exit(0);
	}
	else if (argc != 11 && argc!= 13 && argc != 15 && argc != 17)
	{
		print_error(1, std::to_string(argc - 1));
	}
	else
	{
		for (int i = 1; i < argc; i += 2)
		{
			if (!strcmp(argv[i], "-N"))
				cli.iterations = get_iterations(argv[i + 1]);

			else if (!strcmp(argv[i], "-OG"))
				cli.G_output = argv[i + 1];

			else if (!strcmp(argv[i], "-OE"))
				cli.E_output = argv[i + 1];

			else if (strlen(argv[i]) == 2 && argv[i][0] == '-' && is_file(argv[i + 1]) )
				get_file_values(argv[i][1], argv[i + 1], am);

			else
				print_error(2, argv[i]);
		}
	}
	
	return cli;
}

//Prints out G and E matrices in separate files.
void print_results(std::pair<Matrix, Matrix>& results, std::string G_filename, std::string E_filename)
{
	std::ofstream G_out(G_filename);
	std::ofstream E_out(E_filename);

	G_out << results.first;
	E_out << results.first;
}
