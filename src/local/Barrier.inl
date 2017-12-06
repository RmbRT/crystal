#include <thread>
namespace crystal::local
{
	Barrier::Barrier(
		std::size_t counter):
		m_counter(counter)
	{
	}

	void Barrier::notify()
	{
		m_counter.fetch_sub(1, std::memory_order_relaxed);
	}

	void Barrier::wait()
	{
		while(m_counter.load(std::memory_order_relaxed))
			std::this_thread::yield();
	}
}