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

		/** Decreases the counter. */
		inline void notify();
		/** Waits until the counter is 0. */
		inline void wait();
	};
}

#include "Barrier.inl"

#endif