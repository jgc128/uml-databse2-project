#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <string>

#include "utils.h"
#include "time_measurer.h"
#include "quick_sort.h"
#include "random_number_generator.h"
#include "ext_merge_sort.h"

using namespace std;

void do_quicksort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit)
{
	// load file to perform quicksort
	auto numbers = load_file(input_filename);

	cout << padded_string("Numbers:") << numbers.size() << endl;

	TimeMeasurer time_measurer = TimeMeasurer();
	time_measurer.start();

	auto sorting = QuickSort<long long>();

	sorting.sort(numbers);

	auto time_taken = time_measurer.end();
	auto num_passes = sorting.number_passes();

	cout << padded_string("Sorted:") << num_passes << "ps, " << time_taken << "ms" << endl;
}

void do_merge_sort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit)
{

}

int main(int argc, char **argv) {

	if (argc != 5)
	{
		cerr << "Usage: " << argv[0] << " <input_file> <output_file> <order:0|1> <memory_limit, Mb>" << endl;
		return 1;
	}

	auto input_filename = argv[1];
	auto output_filename = argv[2];
	auto order = strtoul(argv[3], NULL, 0);
	auto memory_limit = strtoul(argv[4], NULL, 0);

	cout << padded_string("Input:") << input_filename << endl;
	cout << padded_string("Output:") << output_filename << endl;
	cout << padded_string("Order:") << order << endl;
	cout << padded_string("Memory limit:") << memory_limit << endl;


	do_quicksort(input_filename, output_filename, order, memory_limit);
	//do_merge_sort(input_filename, output_filename, order, memory_limit);


	int any_key;
	cin >> any_key;

	return 0;
}
