#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <functional>
#include <numeric>

#include "utils.h"
#include "quick_sort.h"
#include "record_io.h"

using namespace std;

#define PAGE_SIZE (4 * 1024) // page size is 4K

struct Run {
	unsigned long pos;
	size_t records;
};
inline bool operator < (const Run & left, const Run & right)
{
	return left.pos < right.pos;
}
inline bool operator > (const Run & left, const Run & right)
{
	return operator < (right, left);
}
inline bool operator == (const Run & left, const Run & right)
{
	return left.pos == right.pos;
}


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

	size_t records_in_page();
	size_t pages_in_memory();

	queue<Run> create_runs();
	void merge(queue<Run> & runs, size_t n);

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

template<typename T, unsigned long RS> size_t ExternalMergeSort<T, RS>::records_in_page()
{
	return PAGE_SIZE / RS; // 1; //
}
template<typename T, unsigned long RS> size_t ExternalMergeSort<T, RS>::pages_in_memory()
{
	return (memory_limit * 1024 * 1024) / PAGE_SIZE; // 3; //
}


template<typename T, unsigned long RS> void ExternalMergeSort<T, RS>::sort()
{
	auto runs = create_runs();
	cout << padded_string("Runs:") << runs.size() << endl;

	cout << endl;

	auto nb_runs_to_merge = pages_in_memory() - 1;
	auto i = 0;
	while (runs.size() > 0) 
	{
		cout << padded_string("Merge:") << i << endl;
		merge(runs, nb_runs_to_merge);
		cout << endl;

		i++;
	}
}

template<typename T, unsigned long RS> void ExternalMergeSort<T, RS>::merge(queue<Run> & runs, size_t n)
{
	QuickSort<Run> quick_sort;

	n = min(n, runs.size());

	vector<Run> runs_to_merge(n);
	for (unsigned i = 0; i < n; i++)
	{
		auto r = runs.front();
		runs_to_merge[i] = r;
		runs.pop();
	}

	// check compatibility for runs
	if (
		runs.size() != 0 
		&& runs_to_merge.size() > 1
		&& (runs_to_merge[0].pos + runs_to_merge[0].records != runs_to_merge[1].pos)
	)
	{
		auto r = runs.front();
		runs.pop();
		runs.push(runs_to_merge[0]);

		for (unsigned i = 1; i < n; i++)
		{
			runs_to_merge[i - 1] = runs_to_merge[i];
		}

		runs_to_merge[n-1] = r;
	}

	// sort runs
	quick_sort.sort(runs_to_merge);

	cout << padded_string("Runs:");
	for (const auto & r : runs_to_merge)
	{
		cout << r.pos << ":" << r.records << " ";
	}
	cout << endl;

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

	// merge runs
	tmp_file.set_write_postion(0);
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

	// create a run for the sorted records
	auto new_run_pos = runs_to_merge[0].pos;
	auto new_run_records = runs_to_merge[runs_to_merge.size() - 1].pos + runs_to_merge[runs_to_merge.size() - 1].records - runs_to_merge[0].pos;
	if (runs.size() != 0)
	{
		Run r = { new_run_pos, new_run_records };
		runs.push(r);
	}

	// write the content of the tmp file to the output file

	output.set_write_postion(new_run_pos);
	tmp_file.set_read_postion(0);
	unsigned long records_to_read = records_in_page() * pages_in_memory();
	unsigned long cur_records = 0;
	unsigned long cur_pos = 0;
	while(cur_pos < new_run_records)
	{
		// for the last batch we do not want to read more than the run actually contains
		auto records = tmp_file.read_records(min(new_run_records - cur_pos, records_to_read));

		cur_records = records.size();

		if (cur_records != 0) {
			output.write_records(records);
		}

		cur_pos += cur_records;
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

