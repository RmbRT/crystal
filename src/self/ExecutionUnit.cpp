#include "ExecutionUnit.hpp"

namespace crystal::self
{
	ExecutionUnit::ExecutionUnit():
		m_handle(),
		m_should_stop(false)
	{
	}

	ExecutionUnit::~ExecutionUnit()
	{
		stop();
	}

	void ExecutionUnit::run(
		lock::ThreadSafe<util::BoundedQueue<Job>> * jobs,
		std::atomic_size_t * available_jobs)
	{
		if(!m_handle.joinable())
			m_handle = std::thread(
				ExecutionUnit::handle_requests,
				this,
				jobs,
				available_jobs);
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
		std::atomic_size_t * available_jobs)
	{
		// the number of failed spins before sleeping.
		static std::size_t const k_sleep_fails = 1000;
		// the sleep duration in microseconds.
		static std::size_t const k_sleep_us = 100;

		std::size_t fails = 0;
		Job job;
		while(!unit->m_should_stop.load(std::memory_order_relaxed))
		{
			if(!available_jobs->load(std::memory_order_relaxed))
			{
				++fails;
				if(fails < k_sleep_fails)
					std::this_thread::yield();
				else
					std::this_thread::sleep_for(
						std::chrono::microseconds(k_sleep_us));
				continue;
			}

			bool found = false;
			{
				auto write = jobs->write();
				if(!write->empty())
				{
					job = write->pop();
					available_jobs->fetch_sub(1, std::memory_order_relaxed);
					found = true;
				} else ++fails;
			}

			if(found)
				job.execute();
			else if(fails < k_sleep_fails)
				std::this_thread::yield();
			else
				std::this_thread::sleep_for(
					std::chrono::microseconds(k_sleep_us));
		}
	}
}