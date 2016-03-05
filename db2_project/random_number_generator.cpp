#include "random_number_generator.h"

default_random_engine RandomNumberGenerator::generator = default_random_engine(1337); // for reproducibility
uniform_real_distribution<double> RandomNumberGenerator::distribution = uniform_real_distribution<double>();
