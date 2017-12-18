#ifndef __crystal_self_machine_hpp_defined
#define __crystal_self_machine_hpp_defined

#include "../Machine.hpp"
#include "../util/ByteOrder.hpp"

namespace crystal::self
{
	/** Representation of the host machine. */
	class Machine : public crystal::Machine
	{
	public:
		/** Creates an uninitialised host machine. */
		Machine();
	};
}

#endif