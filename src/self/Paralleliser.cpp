#include "Paralleliser.hpp"

namespace crystal::self
{
	Paralleliser Paralleliser::s_instance;

	Paralleliser::Paralleliser():
		m_jobs(Paralleliser::hardware_concurrency()-1),
		m_execution(Paralleliser::hardware_concurrency()-1),
		m_available_jobs(0)
	{
		for(auto & unit : m_execution)
			unit.run(&m_jobs, &m_available_jobs);
	}

	void Paralleliser::execute(
		std::function<void (void)> task,
		Barrier * barrier,
		bool force_async)
	{
		do {
			// fast availability check.
			if(s_instance.m_available_jobs.load(std::memory_order_relaxed) != s_instance.m_execution.size())
			{
				auto write = s_instance.m_jobs.write();

				if(!write->full())
				{
					if(barrier)
						write->emplace(
							std::move(task),
							*barrier);
					else
						write->emplace(
							std::move(task));

					s_instance.m_available_jobs.fetch_add(1, std::memory_order_relaxed);

					return;
				}
			}
		} while(force_async);

		task();
		if(barrier)
			barrier->notify();
	}
}