#include "Machine.hpp"

namespace crystal::local
{
	Machine::Machine(
		remote::SerialisedConnection && connection):
		remote::Machine(
			Location::kLocal,
			std::move(connection))
	{
	}
}