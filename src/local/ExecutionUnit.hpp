#ifndef __crystal_local_executionunit_hpp_defined
#define __crystal_local_executionunit_hpp_defined

#include "Job.hpp"

#include "../util/BoundedQueue.hpp"

#include <thread>
#include <Lock/Lock.hpp>

namespace crystal::local
{
	class ExecutionUnit
	{
		std::thread m_handle;
		std::atomic<bool> m_should_stop;
	public:
		ExecutionUnit();
		~ExecutionUnit();

		void run(
			lock::ThreadSafe<util::BoundedQueue<Job>> * jobs,
			std::atomic_size_t * available_jobs);
		void stop();
	private:
		static void handle_requests(
			ExecutionUnit * unit,
			lock::ThreadSafe<util::BoundedQueue<Job>> * jobs,
			std::atomic_size_t * available_jobs);
	};
}

#endif