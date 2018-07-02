#ifndef __crystal_local_machine_hpp_defined
#define __crystal_local_machine_hpp_defined

#include "../remote/Machine.hpp"

namespace crystal::local
{
	/** A trusted machine in the local network.
		This machine does not need encryption. */
	class Machine : public remote::Machine
	{
	public:
		/** Creates a local machine from an incoming connection.
		@param[in] connection:
			The connection to the local machine. */
		Machine(
			remote::SerialisedConnection && connection);

		/** Connects to a local machine.
		@param[in] address:
			The local machine's address. */
		Machine(
			netlib::SocketAddress const& address);
	};
}

#endif