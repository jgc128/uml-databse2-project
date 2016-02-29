#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <utility> 

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

/* ======================================
=========================================
========================================*/



/* =======================================
=== Random Number Generator ==============
========================================*/

default_random_engine RandomNumberGenerator::generator = default_random_engine(1337); // for reproducibility
uniform_real_distribution<double> RandomNumberGenerator::distribution = uniform_real_distribution<double>();

/* ======================================
=========================================
========================================*/



/* =======================================
=== QuickSort ============================
========================================*/



/* ======================================
=========================================
========================================*/




/* =======================================
=== TimeMeasurer =========================
========================================*/

TimeMeasurer::TimeMeasurer()
{
	this->start_time = clock();
}
void TimeMeasurer::start()
{
	this->start_time = clock();
}
double TimeMeasurer::end()
{
	this->end_time = clock();

	return this->get_duration();

}
double TimeMeasurer::get_duration()
{
	return 1000.0 * (end_time - start_time) / CLOCKS_PER_SEC;
}

/* ======================================
=========================================
========================================*/