#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>

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

template<typename T> 
class QuickSort
{
protected:
	unsigned long long partition_calls;
	unsigned long long quicksort_calls;

	size_t partition(vector<T> &data, size_t low, size_t high);
	void quicksort(vector<T> &data, size_t low, size_t high);
public:
	void sort(vector<T> &data);
	void sort(vector<T> &data, size_t low, size_t high);

	unsigned long long number_passes();
};


template<typename T> size_t QuickSort<T>::partition(vector<T> &data, size_t low, size_t high)
{
	partition_calls++;

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

template<typename T> void QuickSort<T>::quicksort(vector<T> &data, size_t low, size_t high)
{
	quicksort_calls++;

	if (low < high)
	{
		auto p = partition(data, low, high);
		quicksort(data, low, p);
		quicksort(data, p + 1, high);
	}
}

template<typename T> void QuickSort<T>::sort(vector<T> &data)
{
	sort(data, 0, data.size() - 1);
}

template<typename T> void QuickSort<T>::sort(vector<T> &data, size_t low, size_t high)
{
	partition_calls = 0;
	quicksort_calls = 0;

	quicksort(data, low, high);
}

template<typename T> unsigned long long QuickSort<T>::number_passes()
{
	return quicksort_calls;
}

/* ======================================
=========================================
========================================*/




/* =======================================
=== TimeMeasurer =========================
========================================*/

class TimeMeasurer
{
protected:
	clock_t start_time;
	clock_t end_time;
public:
	TimeMeasurer();

	void start();
	double end();
	double get_duration();
};

/* ======================================
=========================================
========================================*/