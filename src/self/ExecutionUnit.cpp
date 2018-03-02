#include "ExecutionUnit.hpp"

namespace crystal::self
{
	ExecutionUnit::ExecutionUnit():
		m_handle()
	{
	}

	ExecutionUnit::~ExecutionUnit()
	{
		stop();
	}

	void ExecutionUnit::run(
		lock::ThreadSafe<util::BoundedQueue<Job>> * jobs,
		std::atomic_size_t * available_jobs,
		Spinner const& spinner)
	{
		if(!m_handle.joinable())
			m_handle = std::thread(
				ExecutionUnit::handle_requests,
				this,
				jobs,
				available_jobs,
				spinner);
		else
			throw std::runtime_error("already running.");
	}

	void ExecutionUnit::stop()
	{
		m_should_stop.store(true, std::memory_order_relaxed);
		if(m_handle.joinable())
			m_handle.join();
	}

	void ExecutionUnit::handle_requests(
		ExecutionUnit * unit,
		lock::ThreadSafe<util::BoundedQueue<Job>> * jobs,
		std::atomic_size_t * available_jobs,
		Spinner spinner)
	{
		// make sure that the stop flag is cleared.s
		unit->m_should_stop.store(false, std::memory_order_relaxed);

		Job job;
		while(!unit->m_should_stop.load(std::memory_order_relaxed))
		{
			if(!spinner.spin(available_jobs->load(std::memory_order_relaxed)))
				continue;

			bool found = false;
			{
				auto write = jobs->write();
				if(!write->empty())
				{
					job = write->pop();
					available_jobs->fetch_sub(1, std::memory_order_relaxed);
					found = true;
				}
			}

			if(spinner.spin(found))
				job.execute();
		}
	}
}