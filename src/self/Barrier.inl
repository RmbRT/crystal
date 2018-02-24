#include <thread>
namespace crystal::self
{
	Barrier::Barrier(
		std::size_t counter):
		m_counter(counter)
	{
	}

	void Barrier::notify(
		bool release)
	{
		std::memory_order const& memory_order = release
			? std::memory_order_acquire
			: std::memory_order_relaxed;

		m_counter.fetch_sub(1, memory_order);
	}

	void Barrier::wait(
		bool acquire)
	{
		std::memory_order const& memory_order = acquire
			? std::memory_order_acquire
			: std::memory_order_relaxed;

		while(m_counter.load(memory_order))
			std::this_thread::yield();
	}
}