#ifndef __crystal_slavepool_hpp_defined
#define __crystal_slavepool_hpp_defined

#include "Location.hpp"
#include "local/Paralleliser.hpp"

namespace crystal
{
	class SlavePool
	{
		local::Paralleliser m_local;
	public:
		void execute(
			Location locality,
			Job &job);
	};
}

#endif