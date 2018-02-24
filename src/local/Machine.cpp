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

	Machine::Machine(
		netlib::SocketAddress const& address):
		remote::Machine(
			Location::kLocal,
			address)
	{
	}
}