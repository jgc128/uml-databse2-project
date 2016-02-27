//#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream> 
#include <string>

using namespace std;

int main(int argc, char **argv) {

	if (argc != 5)
	{
		cerr << "Usage: " << argv[0] << " <input_file> <output_file> <order:0|1> <memory_limit, Mb>" << endl;
		return 1;
	}

	auto input_filename = argv[1];
	auto output_filename = argv[2];
	auto order = atoi(argv[3]);
	auto memory_limit = atoi(argv[4]);

	printf("Input:\t\t%s\n", input_filename);
	printf("Output:\t\t%s\n", output_filename);
	printf("Order:\t\t%d\n", order);
	printf("Memory limit:\t%d\n", memory_limit);


	return 0;
}
