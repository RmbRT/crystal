#ifndef __crystal_local_machine_hpp_defined
#define __crystal_local_machine_hpp_defined

#include "../remote/Machine.hpp"

namespace crystal::local
{
	/** A machine in the local network. */
	class Machine : public remote::Machine
	{
	public:
		/** Creates a local machine from an incoming connection.
		@param[in] connection:
			The connection to the local machine. */
		Machine(
			netlib::x::Connection && connection);
		/** Connects to a local machine.
		@param[in] address:
			The local machine's address. */
		Machine(
			netlib::SocketAddress const& address);

		using remote::Machine::send;
		using remote::Machine::receive;
		using remote::Machine::receive_pending;
	};
}

#endif