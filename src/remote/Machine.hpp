#ifndef __crystal_remote_machine_hpp_defined
#define __crystal_remote_machine_hpp_defined

#include "Machine.hpp"
#include <netlib/x/Connection.hpp>

#include "../util/ByteOrder.hpp"

namespace crystal::remote
{
	/** A machine other than the host machine. */
	class Machine : public crystal::Machine, private netlib::x::Connection
	{
		/** The machine's byte order. */
		util::ByteOrder m_byte_order;
	public:
		/** Creates a remote machine.
		@param[in] location:
			The machine's location. Must be either `Location::kArea` or `Location::kOutside`.
		@param[in] connection:
			A connection to the machine. */
		Machine(
			Location location,
			netlib::x::Connection && connection);

		/** The machine's byte order. */
		inline util::ByteOrder byte_order() const;
	};
}

#endif