#include "Barrier.hpp"

namespace crystal::self
{
	Spinner Barrier::s_wait_spinner { 150, 350, 350 };

	void Barrier::configure_spinner(
		std::size_t yield_count,
		std::size_t sleep_count,
		std::size_t sleep_us)
	{
		s_wait_spinner = Spinner(yield_count, sleep_count, sleep_us);
	}

	void Barrier::wait() noexcept
	{
		std::memory_order const& memory_order = m_synchronise
			? std::memory_order_acquire
			: std::memory_order_relaxed;

		Spinner s { s_wait_spinner };
		while(!s.spin(!m_counter.load(memory_order)));
	}

	void Barrier::wait(
		std::size_t yield_count,
		std::size_t sleep_count,
		std::size_t sleep_us) noexcept
	{
		std::memory_order const& memory_order = m_synchronise
			? std::memory_order_acquire
			: std::memory_order_relaxed;

		Spinner s { yield_count, sleep_count, sleep_us };
		while(!s.spin(!m_counter.load(memory_order)));
	}
}