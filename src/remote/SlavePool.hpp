#ifndef __crystal_slavepool_hpp_defined
#define __crystal_slavepool_hpp_defined

#include "Location.hpp"
#include "local/Paralleliser.hpp"

namespace crystal
{
	/** */
	class SlavePool
	{
		/** The slaves. */
		std::vector<Slave> m_slaves;
	public:
	};
}

#endif