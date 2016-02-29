#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <string>

#include "utils.h"

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

	cout << "Input: " << setw(15) << input_filename << endl;
	cout << "Output: " << output_filename << endl;
	cout << "Order: " << order << endl;
	cout << "Memory limit: " << memory_limit << endl;


	// load file to perform quicksort
	auto numbers = load_file(input_filename);

	cout << "Numbers: " << numbers.size() << endl;


	TimeMeasurer time_measurer = TimeMeasurer();
	time_measurer.start();

	auto sorting = QuickSort<long long>();

	sorting.sort(numbers);

	auto time_taken = time_measurer.end();
	auto num_passes = sorting.number_passes();

	cout << "Sorted: " << num_passes << " passes, " << time_taken << "ms" << endl;



	int any_key;
	cin >> any_key;

	return 0;
}
