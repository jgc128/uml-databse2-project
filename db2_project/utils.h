#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

using namespace std;


/* =======================================
=== File IO Utilities ====================
========================================*/

vector<long long> load_file(string filename);



/* ======================================
=========================================
========================================*/




/* =======================================
=== Random Number Generator ==============
========================================*/

class RandomNumberGenerator
{
protected:
	static default_random_engine generator;
	static uniform_real_distribution<double> distribution;

public:
	template<typename T> static T get_random_number(T from, T to);
};

template<typename T> T RandomNumberGenerator::get_random_number(T from, T to)
{
	auto number = distribution(generator);
	return (T)(number * (to - from) + from);
}

/* ======================================
=========================================
========================================*/


/* =======================================
=== QuickSort ============================
========================================*/

template<typename T> void quicksort(vector<T> &data);
template<typename T> void quicksort(vector<T> &data, size_t low, size_t high);
template<typename T> size_t quicksort_partition(vector<T> &data, size_t low, size_t high);

template<typename T> void quicksort(vector<T> &data)
{
	quicksort(data, 0, data.size() - 1);
}

template<typename T> void quicksort(vector<T> &data, size_t low, size_t high)
{
	if (low < high)
	{
		auto p = quicksort_partition(data, low, high);
		quicksort(data, low, p);
		quicksort(data, p + 1, high);
	}
}

template<typename T> size_t quicksort_partition(vector<T> &data, size_t low, size_t high)
{
	auto pivot_idx = RandomNumberGenerator::get_random_number(low, high);

	swap(data[low], data[pivot_idx]);
	auto pivot = data[low];

	auto l = low - 1;
	auto r = high + 1;

	while (true)
	{
		do {
			l++;
		} while (data[l] < pivot);

		do {
			r--;
		} while (data[r] > pivot);

		if (l >= r)
			return r;;

		swap(data[l], data[r]);
	}

}

/* ======================================
=========================================
========================================*/