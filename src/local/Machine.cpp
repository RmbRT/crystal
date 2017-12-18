#include "Machine.hpp"

namespace crystal::local
{
	Machine::Machine(
		netlib::x::Connection && connection):
		remote::Machine(
			Location::kLocal,
			std::move(connection))
	{
	}
}