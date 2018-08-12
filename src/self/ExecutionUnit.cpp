#include "ExecutionUnit.hpp"

namespace crystal::self
{
	ExecutionUnit::ExecutionUnit():
		m_handle(),
		m_should_stop(false),
		m_busy(false)
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
		if(!running())
			m_handle = std::thread(
				ExecutionUnit::handle_requests,
				this,
				jobs,
				available_jobs,
				spinner);
		else
			throw std::runtime_error("already running.");
	}

	void ExecutionUnit::request_stop()
	{
		m_should_stop.store(true, std::memory_order_relaxed);
	}

	void ExecutionUnit::stop()
	{
		if(running())
		{
			m_should_stop.store(true, std::memory_order_relaxed);
			m_handle.join();
		}
	}

	void ExecutionUnit::handle_requests(
		ExecutionUnit * unit,
		lock::ThreadSafe<util::BoundedQueue<Job>> * jobs,
		std::atomic_size_t * available_jobs,
		Spinner spinner)
	{
		// Make sure that the stop flag is cleared.
		unit->m_should_stop.store(false, std::memory_order_relaxed);
		unit->m_busy.store(false, std::memory_order_relaxed);

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
			{
				unit->m_busy.store(true, std::memory_order_relaxed);
				job.execute();
				unit->m_busy.store(false, std::memory_order_relaxed);
			}
		}
	}
}