#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <climits>

#include "quick_sort.h"

using namespace std;

#define PAGE_SIZE (4 * 1024) // page size is 4K

template<typename T>
class ExternalMergeSort{
protected:

	// Get records sizes etc
	size_t record_size();
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
	vector<T> read_records(unsigned long n, streampos start_pos);
	vector<T> read_records(unsigned long n, streamoff offset, ios_base::seekdir way);

	vector<size_t> generate_runs();


public:
	ExternalMergeSort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit);
	~ExternalMergeSort();

	void sort();

};


template<typename T> ExternalMergeSort<T>::ExternalMergeSort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit)
{
	this->order = order;
	this->memory_limit = memory_limit;


	input.open(input_filename);
}
template<typename T> ExternalMergeSort<T>::~ExternalMergeSort()
{
	input.close();
	output.close();
}


template<typename T> size_t ExternalMergeSort<T>::record_size()
{
	return sizeof(T);
}
template<typename T> unsigned long ExternalMergeSort<T>::records_in_page()
{
	return PAGE_SIZE / record_size();
}
template<typename T> unsigned long ExternalMergeSort<T>::pages_in_memory()
{
	return (memory_limit * 1024 * 1024) / PAGE_SIZE;
}


template<typename T> void ExternalMergeSort<T>::sort()
{
	auto runs_boundaries = generate_runs();
}

template<typename T> vector<size_t> ExternalMergeSort<T>::generate_runs()
{
	auto nb_records_to_read = records_in_page() * pages_in_memory();
	

	auto records1 = read_records(3);
	auto records2 = read_records(3);


	return vector<size_t>(10);
}

template<typename T> vector<T> ExternalMergeSort<T>::read_records()
{
	return read_records(0);
}

template<typename T> vector<T> ExternalMergeSort<T>::read_records(unsigned long n)
{
	vector<T> result;
	if (n != 0)
		result.resize(n);

	string input_str;
	T input_val;

	unsigned long cur_record = 0;
	while (false == input.eof() && (cur_record < n || n == 0))
	{
		getline(input, input_str);
		stringstream convert(input_str);
		convert >> input_val;

		if (n == 0)
			result.push_back(input_val);
		else
			result[cur_record] = input_val;

		cur_record++;
	}

	return result;
}

template<typename T> vector<T> ExternalMergeSort<T>::read_records(unsigned long n, streampos start_pos)
{
	input.seekg(start_pos);
	return read_records(n);
}
template<typename T> vector<T> ExternalMergeSort<T>::read_records(unsigned long n, streamoff offset, ios_base::seekdir way)
{
	input.seekg(offset, way);
	return read_records(n);
}
