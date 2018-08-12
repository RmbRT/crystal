#ifndef __crystal_self_executionunit_hpp_defined
#define __crystal_self_executionunit_hpp_defined

#include "Job.hpp"
#include "Spinner.hpp"

#include "../util/BoundedQueue.hpp"

#include <thread>
#include <Lock/Lock.hpp>

namespace crystal::self
{
	class ExecutionUnit
	{
		/** The handle to the execution unit's worker thread. */
		std::thread m_handle;
		/** Whether the worker thread should stop. */
		std::atomic<bool> m_should_stop;
		/** Whether the worker is currently handling a task. */
		std::atomic<bool> m_busy;
	public:
		/** Initialises the execution unit but does not start the worker thread. */
		ExecutionUnit();
		/** Calls `stop()`. */
		~ExecutionUnit();

		/** Runs a worker thread in the background, waiting for new jobs.
			The worker thread runs until `stop()` is called.
		@param[in] jobs:
			The job queue.
		@param[in] available_jobs:
			The number of jobs available in the queue. This parameter is an optimisation as it reduces memory synchronisation overhead.
		@param[in] spinner:
			The spinner controlling the yielding and sleeping when no jobs are present.
		@throws std::runtime_error
			If the background thread is already running. */
		void run(
			lock::ThreadSafe<util::BoundedQueue<Job>> * jobs,
			std::atomic_size_t * available_jobs,
			Spinner const& spinner);

		/** If the worker thread is running, signals it to stop, but does not wait for it. */
		void request_stop();
		/** If the worker thread is running, signals it to stop and waits for it to finish. */
		void stop();

		/** Checks whether the execution unit is working. */
		inline bool running() const;
		/** Checks whether the execution unit is currently busy with a job. */
		inline bool busy() const;
	private:
		/** The worker thread function.
		@param[in] unit:
			The execution unit the worker thread belongs to.
		@param[in] jobs:
			The job queue.
		@param[in] available_jobs:
			The available job count.
		@param[in] spinner:
			The spinner controlling the yielding and sleeping when no jobs are present. */
		static void handle_requests(
			ExecutionUnit * unit,
			lock::ThreadSafe<util::BoundedQueue<Job>> * jobs,
			std::atomic_size_t * available_jobs,
			Spinner spinner);
	};
}

#include "ExecutionUnit.inl"

#endif