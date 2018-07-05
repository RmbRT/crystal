#include "Machine.hpp"

namespace crystal::remote
{
	Machine::Machine(
		Location location,
		SerialisedConnection && connection):
		crystal::Machine(location),
		SerialisedConnection(std::move(connection))
	{
	}
}