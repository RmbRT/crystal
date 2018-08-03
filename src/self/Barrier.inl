#include <thread>
#include "Spinner.hpp"
namespace crystal::self
{
	Spinner const& Barrier::spinner()
	{
		return s_wait_spinner;
	}

	Barrier::Barrier(
		std::size_t counter,
		bool synchronise) noexcept:
		m_counter(counter),
		m_synchronise(synchronise)
	{
	}

	bool Barrier::synchronises() const noexcept
	{
		return m_synchronise;
	}

	void Barrier::notify() noexcept
	{
		std::memory_order const& memory_order = m_synchronise
			? std::memory_order_release
			: std::memory_order_relaxed;

		m_counter.fetch_sub(1, memory_order);
	}

	bool Barrier::finished() const noexcept
	{
		std::memory_order const& memory_order = m_synchronise
			? std::memory_order_acquire
			: std::memory_order_relaxed;

		return m_counter.load(memory_order) == 0;
	}
}