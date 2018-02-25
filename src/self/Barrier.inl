#include <thread>
namespace crystal::self
{
	Barrier::Barrier(
		std::size_t counter,
		bool synchronise) noexcept:
		m_counter(counter),
		m_synchronise(synchronise)
	{
	}

	void Barrier::notify() noexcept
	{
		std::memory_order const& memory_order = m_synchronise
			? std::memory_order_release
			: std::memory_order_relaxed;

		m_counter.fetch_sub(1, memory_order);
	}

	void Barrier::wait() noexcept
	{
		std::memory_order const& memory_order = m_synchronise
			? std::memory_order_acquire
			: std::memory_order_relaxed;

		while(m_counter.load(memory_order))
			std::this_thread::yield();
	}
}