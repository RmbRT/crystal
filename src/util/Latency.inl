#include <utility>

namespace crystal::util
{
	template<class Callable, class ...Args>
	latency_t latency(
		Callable c,
		Args &&... args)
	{
		Timer t;
		t.start();
		c(std::forward<Args>(args)...);
		return t.stop();
	}

	template<class Time, class Duration, class Clock>
	void Timer<Time, Duration, Clock>::start()
	{
		m_start = m_stop = Clock::now();
	}

	template<class Time, class Duration, class Clock>
	Duration Timer<Time, Duration, Clock>::stop()
	{
		m_stop = Clock::now();

		return elapsed();
	}

	template<class Time, class Duration, class Clock>
	Duration Timer<Time, Duration, Clock>::elapsed() const
	{
		return std::chrono::duration<Duration, typename Time::period>{m_stop - m_start}.count();
	}
}