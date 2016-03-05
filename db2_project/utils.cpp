#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "utils.h"

using namespace std;

/* =======================================
=== File IO Utilities ====================
========================================*/

vector<long long> load_file(string filename)
{
	ifstream file(filename);
	vector<long long> results;

	if (file.is_open())
	{
		long long number;
		string line;

		while (getline(file, line))
		{
			number = stoll(line);
			results.push_back(number);
		}

		file.close();
	}
	else
	{
		cerr << "Unable to open file " << filename << endl;
	}

	return results;
}

