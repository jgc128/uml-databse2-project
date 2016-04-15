#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "utils.h"

using namespace std;

ostream & operator << (ostream & os, const SortOrder & order)
{
	os << (order == SortOrder::Ascending ? "Ascending" : "Descending");
	return os;
}

bool is_file_exist(const char *filename)
{
    std::ifstream infile(filename);
    return infile.good();
}

