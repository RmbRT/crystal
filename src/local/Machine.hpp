#ifndef __crystal_local_machine_hpp_defined
#define __crystal_local_machine_hpp_defined

#include "../remote/Machine.hpp"

namespace crystal::local
{
	/** A machine in the local network. */
	class Machine: public remote::Machine
	{
	public:
		Machine() = default;
		Machine(
			netlib::x::Connection && connection);

		using remote::Machine::send;
		using remote::Machine::receive;
	};
}

#endif