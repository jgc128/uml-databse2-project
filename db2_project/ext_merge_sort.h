#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <climits>

#include "quick_sort.h"

using namespace std;

#define PAGE_SIZE (4 * 1024) // page size is 4K

// T - the type of the records
// RS - the size of the records in the file
template<typename T, unsigned long RS> 
class ExternalMergeSort{
protected:

	unsigned long records_in_page();
	unsigned long pages_in_memory();


	ifstream input;
	fstream output;

	// https://en.wikipedia.org/wiki/Seekg
	//fstream myFile("test.txt", ios::in | ios::out | ios::trunc);

	unsigned long order;
	unsigned long memory_limit;

	QuickSort<long long> sorting;

	vector<T> read_records();
	vector<T> read_records(unsigned long n);
	vector<T> read_records(unsigned long n, unsigned long pos);

	vector<size_t> generate_runs();


public:
	ExternalMergeSort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit);
	~ExternalMergeSort();

	void sort();

};


template<typename T, unsigned long RS> ExternalMergeSort<T, RS>::ExternalMergeSort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit)
{
	this->order = order;
	this->memory_limit = memory_limit;


	input.open(input_filename);
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
	auto runs_boundaries = generate_runs();
}

template<typename T, unsigned long RS> vector<size_t> ExternalMergeSort<T, RS>::generate_runs()
{
	auto nb_records_to_read = records_in_page() * pages_in_memory();
	

	auto records1 = read_records(4);
	for (auto &r : records1)
	{
		cout << r << endl;
	}
	cout << endl;



	return vector<size_t>(10);
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
