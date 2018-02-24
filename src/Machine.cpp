#include "Machine.hpp"
#include <cmath>
#include <random>

#include <iostream>
#include "self/Paralleliser.hpp"

namespace crystal
{
	Machine::Machine(
		Location location):
		m_location(location)
	{
	}
	Machine::~Machine() = default;
}