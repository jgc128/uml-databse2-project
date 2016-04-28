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

#define RECORD_TYPE long long
#define RECORD_SIZE 16

#define MAX_MEM 4096

using namespace std;

void do_quicksort(string input_filename, string output_filename, SortOrder order, unsigned long memory_limit)
{
	TimeMeasurer time_measurer;
	time_measurer.start();

	// load the whole file to perform quicksort
	RecordIO<RECORD_TYPE, RECORD_SIZE> input(input_filename);
	auto numbers = input.read_records();

	cout << padded_string("Numbers:") << numbers.size() << endl;


	QuickSort<RECORD_TYPE> sorting;
	sorting.sort(numbers, order);

	// write results
	RecordIO<RECORD_TYPE, RECORD_SIZE> output(output_filename, ios::out | ios::trunc);
	output.write_records(numbers);

	auto time_taken = time_measurer.end();
	auto num_passes = sorting.number_passes();

	cout << padded_string("Sorted:") << num_passes << "ps, " << time_taken << "ms" << endl;
}

void do_merge_sort(string input_filename, string output_filename, SortOrder order, unsigned long memory_limit)
{
	TimeMeasurer time_measurer;
	time_measurer.start();

	ExternalMergeSort<RECORD_TYPE, RECORD_SIZE> ext_sort(input_filename, output_filename, order, memory_limit);
	ext_sort.sort();

	auto time_taken = time_measurer.end();

	cout << padded_string("Sorted:") << time_taken << "ms" << endl;
}

int main(int argc, char **argv) {

	if (argc != 5)
	{
		cerr << "Usage: " << argv[0] << " <input_file> <output_file> <order:0|1> <memory_limit, Mb>" << endl;
		return 1;
	}

	auto input_filename = argv[1];
	auto output_filename = argv[2];
	auto order = strtoul(argv[3], NULL, 0) == 0 ? SortOrder::Ascending : SortOrder::Descending;
	auto memory_limit = strtoul(argv[4], NULL, 0);

	if(memory_limit == 0 || memory_limit > MAX_MEM)
	{
		cerr << "Invalid memory limit" << endl;
		return 1;
	}
	if(!is_file_exist(input_filename))
	{
		cerr << "Invalid input file" << endl;
		return 1;
	}

	cout << padded_string("Input:") << input_filename << endl;
	cout << padded_string("Output:") << output_filename << endl;
	cout << padded_string("Order:") << order << endl;
	cout << padded_string("Memory limit:") << memory_limit << endl;
	cout << endl;

	//do_quicksort(input_filename, output_filename, order, memory_limit);
	do_merge_sort(input_filename, output_filename, order, memory_limit);

	return 0;
}
