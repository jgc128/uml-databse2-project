#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <functional>

#include "utils.h"
#include "quick_sort.h"
#include "record_io.h"

using namespace std;

#define PAGE_SIZE (4 * 1024) // page size is 4K

struct Run {
	unsigned long pos;
	size_t records;
};

template<typename T> struct RecordRunInfo {
	T value;
	unsigned long run;
};
template<typename T> inline bool operator < (const RecordRunInfo<T> & left, const RecordRunInfo<T> & right)
{
	return left.value < right.value;
}
template<typename T> inline bool operator > (const RecordRunInfo<T> & left, const RecordRunInfo<T> & right)
{
	return operator < (right, left);
}
template<typename T> inline bool operator == (const RecordRunInfo<T> & left, const RecordRunInfo<T> & right)
{
	return left.value == right.value;
}


// T - the type of the records
// RS - the size of the records in the file
template<typename T, unsigned long RS> 
class ExternalMergeSort{
protected:
	RecordIO<T, RS> input;
	RecordIO<T, RS> output;
	RecordIO<T, RS> tmp_file;

	unsigned long order;
	unsigned long memory_limit;

	unsigned long records_in_page();
	unsigned long pages_in_memory();

	queue<Run> create_runs();
	void merge(queue<Run> & runs, unsigned long n);

public:
	ExternalMergeSort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit);
	~ExternalMergeSort();

	void sort();

};


template<typename T, unsigned long RS> ExternalMergeSort<T, RS>::ExternalMergeSort(string input_filename, string output_filename, unsigned long order, unsigned long memory_limit)
	: input(input_filename, ios::in)
	, output(output_filename, ios::in | ios::out | ios::trunc)
	, tmp_file(output_filename + "_tmp", ios::in | ios::out | ios::trunc)
{
	this->order = order;
	this->memory_limit = memory_limit;
}
template<typename T, unsigned long RS> ExternalMergeSort<T, RS>::~ExternalMergeSort()
{

}

template<typename T, unsigned long RS> unsigned long ExternalMergeSort<T, RS>::records_in_page()
{
	return 3; //PAGE_SIZE / RS;
}
template<typename T, unsigned long RS> unsigned long ExternalMergeSort<T, RS>::pages_in_memory()
{
	return 1; //(memory_limit * 1024 * 1024) / PAGE_SIZE;
}


template<typename T, unsigned long RS> void ExternalMergeSort<T, RS>::sort()
{
	auto runs = create_runs();
	cout << padded_string("Runs:") << runs.size() << endl;

	merge(runs, 10);


	int i = 0;
}

template<typename T, unsigned long RS> void ExternalMergeSort<T, RS>::merge(queue<Run> & runs, unsigned long n)
{
	vector<Run> runs_to_merge(n);
	for (unsigned i = 0;i < n; i++)
	{
		auto r = runs.front();
		runs_to_merge[i] = r;
		runs.pop();
	}

	priority_queue<RecordRunInfo<T>, vector<RecordRunInfo<T>>, greater<RecordRunInfo<T>>> records;
	
	// set the initial positions of the runs
	vector<unsigned long> runs_postions(n);
	for (unsigned i = 0; i < n; i++)
	{
		runs_postions[i] = runs_to_merge[i].pos;
	}

	// fill the priority queue initially
	for (unsigned i = 0; i < n; i++)
	{
		auto rec = output.read_records(1, runs_postions[i]);
		RecordRunInfo<T> rec_run_info = { rec[0], i };
		records.push(rec_run_info);

		runs_postions[i] += 1;
	}

	while(records.size() != 0)
	{
		auto smallest_rec = records.top();
		records.pop();

		tmp_file.write_record(smallest_rec.value);

		if (runs_postions[smallest_rec.run] < runs_to_merge[smallest_rec.run].pos + runs_to_merge[smallest_rec.run].records)
		{
			auto rec = output.read_records(1, runs_postions[smallest_rec.run]);
			RecordRunInfo<T> rec_run_info = { rec[0], smallest_rec.run };
			records.push(rec_run_info);

			runs_postions[smallest_rec.run] += 1;
		}
	}

}

template<typename T, unsigned long RS> queue<Run> ExternalMergeSort<T, RS>::create_runs()
{
	auto records_to_read = records_in_page() * pages_in_memory();

	QuickSort<T> quick_sort;
	queue<Run> runs;

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
			runs.push(r);

			cout << padded_string("run:") << r.pos << ":" << r.records << endl;
		}

		cur_pos += cur_records;
	} while (cur_records == records_to_read);

	return runs;
}

