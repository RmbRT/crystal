#ifndef __crystal_location_hpp_defined
#define __crystal_location_hpp_defined

namespace crystal
{
	/** An machine's location.
		This determines the level of trust a machine is given and whether communication is required to reach the machine. */
	enum class Location
	{
		/** The host machine.
			Requires no communication and has absolute trust. */
		kSelf,
		/** Machines in the local area network.
			Requires communication and has absolute trust. */
		kLocal,
		/** Machines outside the local are network.
			Requires communication and is not trusted. */
		kOutside
	};
}

#endif