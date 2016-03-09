#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <climits>

#include "quick_sort.h"

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
	ifstream input;
	fstream output;

	unsigned long order;
	unsigned long memory_limit;

	const char fill_char;

	unsigned long records_in_page();
	unsigned long pages_in_memory();

	vector<T> read_records();
	vector<T> read_records(unsigned long n);
	vector<T> read_records(unsigned long n, unsigned long pos);

	void write_records(vector<T> records);
	void write_records(vector<T> records, unsigned long pos);

	vector<Run> create_runs();


public:
	ExternalMergeSort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit);
	~ExternalMergeSort();

	void sort();

};


template<typename T, unsigned long RS> ExternalMergeSort<T, RS>::ExternalMergeSort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit)
	: fill_char('0')
{
	this->order = order;
	this->memory_limit = memory_limit;


	input.open(input_filename);
	output.open(output_filename, ios::in | ios::out | ios::trunc);
}
template<typename T, unsigned long RS> ExternalMergeSort<T, RS>::~ExternalMergeSort()
{
	input.close();
	output.close();
}

template<typename T, unsigned long RS> unsigned long ExternalMergeSort<T, RS>::records_in_page()
{
	return PAGE_SIZE / RS;
}
template<typename T, unsigned long RS> unsigned long ExternalMergeSort<T, RS>::pages_in_memory()
{
	return (memory_limit * 1024 * 1024) / PAGE_SIZE;
}


template<typename T, unsigned long RS> void ExternalMergeSort<T, RS>::sort()
{
	auto runs_boundaries = create_runs();
}

template<typename T, unsigned long RS> vector<Run> ExternalMergeSort<T, RS>::create_runs()
{
	auto records_to_read = 4;// records_in_page() * pages_in_memory();

	QuickSort<T> quick_sort;
	vector<Run> runs;

	unsigned long cur_pos = 0;
	unsigned long cur_records = 0;
	do
	{
		auto records = read_records(records_to_read);

		quick_sort.sort(records);
		write_records(records);

		cur_records = records.size();
		Run r = {
			cur_pos, 
			cur_records
		};
		runs.push_back(r);

		cur_pos += cur_records;
	} while (cur_records == records_to_read);

	return runs;
}

template<typename T, unsigned long RS> vector<T> ExternalMergeSort<T, RS>::read_records()
{
	return read_records(0);
}

template<typename T, unsigned long RS> vector<T> ExternalMergeSort<T, RS>::read_records(unsigned long n)
{
	vector<T> result;
	if (n != 0)
		result.resize(n);

	char input_str[RS + 1]; // +1 for the \n
	T input_val;

	unsigned long cur_record = 0;
	while (false == input.eof() && (cur_record < n || n == 0))
	{
		input.read(input_str, RS + 1);

		stringstream convert(input_str);
		convert >> input_val;

		if (n == 0)
			result.push_back(input_val);
		else
			result[cur_record] = input_val;

		cur_record++;
	}

	// shrink the vector for the last records in the file
	if (n != 0 && cur_record != n)
	{
		result.resize(cur_record);
	}

	return result;
}

template<typename T, unsigned long RS> vector<T> ExternalMergeSort<T, RS>::read_records(unsigned long n, unsigned long pos)
{
	auto real_pos = pos * (RS + 1); // +1 for the \n
	input.seekg(real_pos);

	return read_records(n);
}

template<typename T, unsigned long RS> void ExternalMergeSort<T, RS>::write_records(vector<T> records)
{
	for (T &r : records)
	{
		output << right << setfill(fill_char) << setw(RS) << r << endl;
	}
}

template<typename T, unsigned long RS> void ExternalMergeSort<T, RS>::write_records(vector<T> records, unsigned long pos)
{
	auto real_pos = pos * (RS + 1); // +1 for the \n
	output.seekp(real_pos);

	write_records(records);
}


