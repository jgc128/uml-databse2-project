#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

using namespace std;

vector<long long> load_file(string filename);


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
