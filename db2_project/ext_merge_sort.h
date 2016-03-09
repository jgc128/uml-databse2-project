#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "utils.h"
#include "quick_sort.h"
#include "record_io.h"

using namespace std;

#define PAGE_SIZE (4 * 1024) // page size is 4K

struct Run {
	unsigned long pos;
	size_t records;
};

// T - the type of the records
// RS - the size of the records in the file
template<typename T, unsigned long RS> 
class ExternalMergeSort{
protected:
	RecordIO<T, RS> input;
	RecordIO<T, RS> output;

	unsigned long order;
	unsigned long memory_limit;

	unsigned long records_in_page();
	unsigned long pages_in_memory();

	void print_runs(const vector<Run> & runs);

	vector<Run> create_runs();



public:
	ExternalMergeSort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit);
	~ExternalMergeSort();

	void sort();

};


template<typename T, unsigned long RS> ExternalMergeSort<T, RS>::ExternalMergeSort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit)
	: input(input_filename, ios::in), output(output_filename, ios::in | ios::out | ios::trunc)
{
	this->order = order;
	this->memory_limit = memory_limit;
}
template<typename T, unsigned long RS> ExternalMergeSort<T, RS>::~ExternalMergeSort()
{

}

template<typename T, unsigned long RS> unsigned long ExternalMergeSort<T, RS>::records_in_page()
{
	return PAGE_SIZE / RS;
}
template<typename T, unsigned long RS> unsigned long ExternalMergeSort<T, RS>::pages_in_memory()
{
	return (memory_limit * 1024 * 1024) / PAGE_SIZE;
}

template<typename T, unsigned long RS> void ExternalMergeSort<T, RS>::print_runs(const vector<Run> & runs)
{
	cout << padded_string("Runs:") << runs.size() << endl;
	for (auto &r : runs)
	{
		cout << padded_string("run:") << r.pos << ":" << r.records << endl;
	}
}


template<typename T, unsigned long RS> void ExternalMergeSort<T, RS>::sort()
{
	auto runs = create_runs();
	print_runs(runs);








	int i = 0;
}

template<typename T, unsigned long RS> vector<Run> ExternalMergeSort<T, RS>::create_runs()
{
	auto records_to_read = 4; // records_in_page() * pages_in_memory();

	QuickSort<T> quick_sort;
	vector<Run> runs;

	unsigned long cur_pos = 0;
	unsigned long cur_records = 0;
	do
	{
		auto records = input.read_records(records_to_read);
		cur_records = records.size();

		if (cur_records != 0) {
			quick_sort.sort(records);
			output.write_records(records);

			Run r = { cur_pos, cur_records };
			runs.push_back(r);
		}

		cur_pos += cur_records;
	} while (cur_records == records_to_read);

	return runs;
}

