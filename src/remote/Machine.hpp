#ifndef __crystal_remote_machine_hpp_defined
#define __crystal_remote_machine_hpp_defined

#include "SerialisedConnection.hpp"

#include "../Machine.hpp"

namespace crystal::remote
{
	/** A machine other than the host machine. */
	class Machine : public crystal::Machine, protected SerialisedConnection
	{
	public:
		Machine(Machine&&) = default;

		/** Creates a remote machine from an incoming connection.
		@param[in] location:
			The machine's location. Must be either `Location::kArea` or `Location::kOutside`.
		@param[in] connection:
			A connection to the machine. */
		Machine(
			Location location,
			SerialisedConnection && connection);

		/** Connects to a remote machine.
		@param[in] location:
			The remote machine's location.
		@param[in] address:
			The remote machine's address. */
		Machine(
			Location location,
			netlib::SocketAddress const& address);

		virtual ~Machine() = 0;

		using SerialisedConnection::exists;

		using SerialisedConnection::operator<<;
		using SerialisedConnection::operator>>;

	private:
		/** Sends this machine's byte order.
		@return
			Whether the transfer was successful. */
		bool send_byte_order();
		/** Receives this machine's byte order.
		@return
			Whether the transfer was successful. */
		bool receive_byte_order();
	};
}

#include "Machine.inl"

#endif