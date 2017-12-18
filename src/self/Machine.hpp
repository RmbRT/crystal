#ifndef __crystal_self_machine_hpp_defined
#define __crystal_self_machine_hpp_defined

#include "../Machine.hpp"

namespace crystal::self
{
	/** Representation of the host machine. */
	class Machine : public crystal::Machine
	{
	public:
		/** Creates an uninitialised host machine. */
		Machine();

		/** The host machine's byte order. */
		static constexpr std::uint64_t byte_order();
	};
}

#endif