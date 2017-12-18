#ifndef __crystal_machine_hpp_defined
#define __crystal_machine_hpp_defined

#include "Location.hpp"

#include <cstddef>
#include <cinttypes>
#include <random>

namespace crystal
{
	/** A known machine. */
	class Machine
	{
		/** The machine's network location. */
		Location m_location;
	public:
		/** Creates a machine at the given location. */
		Machine(
			Location location);
		/** Creates a machine at the given location and with the given latencies.
		@param[in] location:
			The machine's location. */
		Machine(
			Location location);

		virtual ~Machine() = 0;

		/** The machine's location. */
		inline Location location() const;
	};
}

#include "Machine.inl"

#endif