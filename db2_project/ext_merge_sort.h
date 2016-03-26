#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <functional>
#include <numeric>
#include <cstdio>

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

	RecordIO<T, RS> * current_output;
	RecordIO<T, RS> * current_tmp;

	unsigned long order;
	unsigned long memory_limit;

	size_t records_in_page();
	size_t pages_in_memory();

	queue<Run> create_runs();
	void merge(queue<Run> & runs, size_t n);

	size_t previous_run_size = 0;
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
	return PAGE_SIZE / RS;
}
template<typename T, unsigned long RS> size_t ExternalMergeSort<T, RS>::pages_in_memory()
{
	return (memory_limit * 1024 * 1024) / PAGE_SIZE;
}


template<typename T, unsigned long RS> void ExternalMergeSort<T, RS>::sort()
{
	auto runs = create_runs();
	cout << padded_string("Runs:") << runs.size() << endl;
	cout << endl;

	current_output = &output;
	current_tmp = &tmp_file;

	auto nb_runs_to_merge = pages_in_memory() - 1;
	auto i = 0;
	while (runs.size() > 0) 
	{
		cout << padded_string("Merge:") << i << endl;
		merge(runs, nb_runs_to_merge);
		cout << endl;

		i++;
	}

	output.close();
	tmp_file.close();
	input.close();

	// rename the output file and remove tmp file
	if (current_output != &tmp_file)
	{
		cout << "Need to exchange the tmp file and the output file" << endl;
		cout << endl;

		remove(output.get_filename().c_str());
		rename(tmp_file.get_filename().c_str(), output.get_filename().c_str());
	}
	else 
	{
		remove(tmp_file.get_filename().c_str());
	}
}

template<typename T, unsigned long RS> void ExternalMergeSort<T, RS>::merge(queue<Run> & runs, size_t n)
{
	n = min(n, runs.size());

	vector<Run> runs_to_merge(n);
	for (unsigned i = 0; i < n; i++)
	{
		auto r = runs.front();

		if (i > 0 && runs_to_merge[0].records < r.records) // the previous run was the last run of the size that we are merging this time
		{
			runs_to_merge.resize(i);
			n = i;
			break;
		}

		runs_to_merge[i] = r;
		runs.pop();
	}

	if (runs_to_merge[0].records > previous_run_size)
	{
		cout << "Change run size: " << previous_run_size << " -> " << runs_to_merge[0].records << endl;

		if(previous_run_size != 0)
			swap(current_output, current_tmp);

		current_tmp->set_write_postion(0);
		current_output->set_read_postion(0);
	}


	cout << padded_string("Runs:");
	for (const auto & r : runs_to_merge)
	{
		cout << r.pos << ":" << r.records << " ";
	}
	cout << endl;

	function<bool(const RecordRunInfo<T> & a, const RecordRunInfo<T> & b)> comp;
	if (order == 0)
		comp = [](const RecordRunInfo<T> & a, const RecordRunInfo<T> & b) -> bool { return a > b; };
	else
		comp = [](const RecordRunInfo<T> & a, const RecordRunInfo<T> & b) -> bool { return a < b; };
		
	priority_queue<RecordRunInfo<T>, vector<RecordRunInfo<T>>, function<bool(const RecordRunInfo<T> & a, const RecordRunInfo<T> & b)>> records(comp);
	
	// set the initial positions of the runs
	vector<unsigned long> runs_postions(n);
	for (unsigned i = 0; i < n; i++)
	{
		runs_postions[i] = runs_to_merge[i].pos;
	}

	// fill the priority queue initially
	for (unsigned i = 0; i < n; i++)
	{
		auto rec = current_output->read_records(1, runs_postions[i]);
		RecordRunInfo<T> rec_run_info = { rec[0], i };
		records.push(rec_run_info);

		runs_postions[i] += 1;
	}

	// merge runs
	while(records.size() != 0)
	{
		auto smallest_rec = records.top();
		records.pop();

		current_tmp->write_record(smallest_rec.value);

		if (runs_postions[smallest_rec.run] < runs_to_merge[smallest_rec.run].pos + runs_to_merge[smallest_rec.run].records)
		{
			auto rec = current_output->read_records(1, runs_postions[smallest_rec.run]);
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

	if(runs_to_merge[0].records > previous_run_size)
		previous_run_size = runs_to_merge[0].records;
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
			quick_sort.sort(records, order);
			output.write_records(records);

			Run r = { cur_pos, cur_records };
			runs.push(r);

			cout << padded_string("run:") << r.pos << ":" << r.records << endl;
		}

		cur_pos += cur_records;
	} while (cur_records == records_to_read);

	return runs;
}

