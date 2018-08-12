#include "Paralleliser.hpp"

namespace crystal::self
{
	Paralleliser Paralleliser::s_instance;

	Paralleliser::Paralleliser():
		m_available_jobs(0),
		m_active_workers(0),
		m_jobs(Paralleliser::hardware_concurrency()-1),
		m_execution(Paralleliser::hardware_concurrency()-1)
	{
	}

	void Paralleliser::execute(
		std::function<void (void)> task,
		Barrier * barrier,
		bool force_async)
	{
		std::size_t const kWorkerSlots = Paralleliser::hardware_concurrency()-1;
		// If all running workers are busy, start an additional worker (if available).
		if(s_instance.m_active_workers.load(std::memory_order_relaxed)
		!= kWorkerSlots)
		{
			for(auto & worker: *s_instance.m_execution.write())
				if(worker.running())
				{
					if(!worker.busy())
						break;
				}
				else
				{
					worker.run(
						&s_instance.m_jobs,
						&s_instance.m_available_jobs,
						Spinner(150, 350, 350));

					s_instance.m_active_workers.fetch_add(1, std::memory_order_relaxed);
					break;
				}
		}

		do {
			// fast availability check.
			// relaxed ordering because the lock will synchronise.
			if(s_instance.m_available_jobs.load(std::memory_order_relaxed)
			!= kWorkerSlots)
			{
				// acquire lock.
				auto write = s_instance.m_jobs.write();

				// Is there still space available after locking?
				if(!write->full())
				{
					if(barrier)
						write->emplace(
							std::move(task),
							*barrier);
					else
						write->emplace(
							std::move(task));

					// increment the number of available jobs.
					// relaxed ordering because the lock synchronises upon destruction.
					s_instance.m_available_jobs.fetch_add(1, std::memory_order_relaxed);

					return;
				}
			}
		} while(force_async);

		// no task was available, so execute the task on the calling thread.
		task();

		// notify the barrier.
		if(barrier)
			barrier->notify();
	}

	void Paralleliser::stop_workers(
		bool wait)
	{
		auto m_execution = s_instance.m_execution.write();
		// First, signal all workers to stop.
		for(auto & execution : *m_execution)
			execution.request_stop();

		s_instance.m_active_workers.store(0, std::memory_order_relaxed);

		// Then, wait for them to stop.
		if(wait)
			for(auto & execution : *m_execution)
				execution.stop();
	}
}