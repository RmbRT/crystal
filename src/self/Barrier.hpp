#ifndef __crystal_self_barrier_hpp_defined
#define __crystal_self_barrier_hpp_defined

#include "Spinner.hpp"

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

		/** The default spinner used by all barriers. */
		static Spinner s_wait_spinner;
	public:

		/** Configures the default spinner used by all barriers.
		@param[in] yield_count:
			How many times the Spinner should spin before yielding the thread.
		@param[in] sleep_count:
			How many times the spinner should spin before sleeping.
		@param[in] sleep_us:
			The sleep duration in microseconds. */
		static void configure_spinner(
			std::size_t yield_count,
			std::size_t sleep_count,
			std::size_t sleep_us);

		/** Retrieves the default spinner. */
		inline static Spinner const& spinner();

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
		void wait() noexcept;

		/** Waits with a custom spinner.
		@param[in] yield_count:
			How many times the Spinner should spin before yielding the thread.
		@param[in] sleep_count:
			How many times the spinner should spin before sleeping.
		@param[in] sleep_us:
			The sleep duration in microseconds. */
		void wait(
			std::size_t yield_count,
			std::size_t sleep_count,
			std::size_t sleep_us) noexcept;

		/** Checks whether the barrier is finished.
		@return
			Whether the barrier is finished. */
		inline bool finished() const noexcept;
	};
}

#include "Barrier.inl"

#endif