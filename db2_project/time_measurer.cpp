#include "time_measurer.h"

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
