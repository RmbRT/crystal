#ifndef __crystal_util_latency_hpp_defined
#define __crystal_util_latency_hpp_defined

#include <chrono>

namespace crystal::util
{
	typedef double latency_t;

	template<class Callable, class ...Args>
	/** The latency of a function call.
	@param[in] c:
		The callable to measure.
	@param[in] args:
		The arguments to pass to c.
	@return
		The elapsed time in seconds. */
	latency_t latency(
		Callable c,
		Args &&... args);

	template<
		class Time = std::chrono::seconds,
		class Duration = latency_t,
		class Clock = std::conditional<
			std::chrono::high_resolution_clock::is_steady,
			std::chrono::high_resolution_clock,
			std::chrono::steady_clock>::type>
	/** Time interval measurement class. */
	class Timer
	{
		/** The moment the timer started recording. */
		std::chrono::time_point<Clock> m_start;
		/** The moment the timer stopped recording. */
		std::chrono::time_point<Clock> m_stop;
	public:
		static_assert(Clock::is_steady, "The chosen clock type is not steady.");

		/** Starts recording time. */
		void start();
		/** Stops recording time.
		@return
			The recorded time interval. */
		Duration stop();
		/** Retrieves the recorded time.
		@return
			The recorded time interval. */
		Duration elapsed() const;
	};
}

#include "Latency.inl"

#endif