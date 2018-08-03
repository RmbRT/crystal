#include "Spinner.hpp"
#include <thread>

namespace crystal::self
{
	Spinner::Spinner(
		std::size_t yield_count,
		std::size_t sleep_count,
		std::size_t sleep_us):
		m_fails(0),
		m_yield_count(yield_count),
		m_sleep_count(sleep_count),
		m_sleep_us(sleep_us)
	{
	}

	bool Spinner::spin(
		bool success)
	{
		if(success)
			m_fails = 0;
		else if(++m_fails > m_sleep_count)
			std::this_thread::sleep_for(
				std::chrono::microseconds(m_sleep_us));
		else if(m_fails > m_yield_count)
			std::this_thread::yield();

		return success;
	}
}