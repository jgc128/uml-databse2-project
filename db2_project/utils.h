#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <iomanip> 
#include <sstream>

using namespace std;


/* =======================================
=== File IO Utilities ====================
========================================*/

vector<long long> load_file(string filename);



/* =======================================
=== Printing Utilities ===================
========================================*/

template<typename T> string padded_string(T data, const unsigned int width = 15, const char separator = ' ');

template<typename T> string padded_string(T data, const unsigned int width, const char separator)
{
	stringstream s;
	s << left << setw(width) << setfill(separator) << data;
	return s.str();
}

