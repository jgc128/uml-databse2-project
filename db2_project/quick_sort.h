#pragma once

#include <vector>

#include "random_number_generator.h"

using namespace std;

template<typename T>
class QuickSort
{
protected:
	unsigned long long partition_calls;
	unsigned long long quicksort_calls;

	size_t partition(vector<T> &data, size_t low, size_t high, unsigned long order);
	void quicksort(vector<T> &data, size_t low, size_t high, unsigned long order);
public:
	void sort(vector<T> &data);
	void sort(vector<T> &data, unsigned long order);
	void sort(vector<T> &data, size_t low, size_t high);
	void sort(vector<T> &data, size_t low, size_t high, unsigned long order);

	unsigned long long number_passes();
};


template<typename T> size_t QuickSort<T>::partition(vector<T> &data, size_t low, size_t high, unsigned long order)
{
	partition_calls++;

	auto pivot_idx = RandomNumberGenerator::get_random_number(low, high);

	swap(data[low], data[pivot_idx]);
	auto pivot = data[low];

	auto l = low - 1;
	auto r = high + 1;


	function<bool(const T & a, const T & b)> comp_l = [](const T & a, const T & b) -> bool { return a < b; };
	function<bool(const T & a, const T & b)> comp_r = [](const T & a, const T & b) -> bool { return a > b; };
	if (order != 0)
		swap(comp_l, comp_r);

	while (true)
	{
		do {
			l++;
		} while (comp_l(data[l], pivot));

		do {
			r--;
		} while (comp_r(data[r], pivot));

		if (l >= r)
			return r;

		swap(data[l], data[r]);
	}
}

template<typename T> void QuickSort<T>::quicksort(vector<T> &data, size_t low, size_t high, unsigned long order)
{
	quicksort_calls++;

	if (low < high)
	{
		auto p = partition(data, low, high, order);
		quicksort(data, low, p, order);
		quicksort(data, p + 1, high, order);
	}
}

template<typename T> void QuickSort<T>::sort(vector<T> &data)
{
	sort(data, 0);
}
template<typename T> void QuickSort<T>::sort(vector<T> &data, unsigned long order)
{
	sort(data, 0, data.size() - 1, order);
}
template<typename T> void QuickSort<T>::sort(vector<T> &data, size_t low, size_t high)
{
	sort(data, low, high, order);
}
template<typename T> void QuickSort<T>::sort(vector<T> &data, size_t low, size_t high, unsigned long order)
{
	partition_calls = 0;
	quicksort_calls = 0;

	quicksort(data, low, high, order);
}

template<typename T> unsigned long long QuickSort<T>::number_passes()
{
	return quicksort_calls;
}
