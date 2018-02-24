#ifndef __crystal_self_barrier_hpp_defined
#define __crystal_self_barrier_hpp_defined

#include <atomic>

namespace crystal::self
{
	/** Synchronisation barrier. */
	class Barrier
	{
		/** The barrier's counter. */
		std::atomic<std::size_t> m_counter;
	public:
		/** Creates a barrier with the requested counter. */
		inline Barrier(
			std::size_t counter);

		/** Decreases the counter.
			Performs either a release operation or a relaxed operation.
		@param[in] release:
			Whether to perform a release operation. */
		inline void notify(
			bool release = false);

		/** Waits until the counter is 0.
			Performs either an acquire operation or a relaxed operation.
		@param[in] acquire:
			Whether to perform an acquire operation. */
		inline void wait(
			bool acquire = false);
	};
}

#include "Barrier.inl"

#endif