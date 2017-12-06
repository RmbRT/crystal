#ifndef __crystal_location_hpp_defined
#define __crystal_location_hpp_defined

namespace crystal
{
	/** An execution location. */
	enum class Location
	{
		/** The host machine. */
		kMachine,
		/** Machines in the local area network. */
		kLocal,
		/** Machines outside the local are network. */
		kOutside
	};
}

#endif