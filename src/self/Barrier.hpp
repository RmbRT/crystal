#ifndef __crystal_self_barrier_hpp_defined
#define __crystal_self_barrier_hpp_defined

#include <atomic>

namespace crystal::self
{
	/** Synchronisation barrier.
		Supports optional memory synchronisation via acquire-release behaviour. */
	class Barrier
	{
		/** The barrier's counter. */
		std::atomic<std::size_t> m_counter;
		/** Whether to synchronise the memory. */
		bool m_synchronise;
	public:
		/** Creates a barrier with the requested counter.
		@param[in] counter:
			How many tasks have to arrive at the barrier to continue.
		@param[in] synchronise:
			Whether to synchronise the memory of all tasks. */
		inline Barrier(
			std::size_t counter,
			bool synchronise) noexcept;

		/** Indicates whether the barrier synchronises memory. */
		inline bool synchronises() const noexcept;

		/** Decreases the counter.
			Performs either a release operation or a relaxed operation, depending on whether the barrier is set to synchronise. */
		inline void notify() noexcept;

		/** Waits until the counter is 0.
			Performs either an acquire operation or a relaxed operation, depending on whether the barrier is set to synchronise. */
		inline void wait() noexcept;
	};
}

#include "Barrier.inl"

#endif