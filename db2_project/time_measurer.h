#pragma once

#include <ctime>

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
