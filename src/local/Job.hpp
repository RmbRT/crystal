#ifndef __crystal_local_job_hpp_defined
#define __crystal_local_job_hpp_defined

#include "Barrier.hpp"

#include <functional>

namespace crystal::local
{
	/** Job function type. */
	typedef std::function<void(void)> job_fn_t;

	/** A job. */
	class Job
	{
		/** The job's code. */
		job_fn_t m_job_function;
		/** An optional barrier to decrease after the job is done. */
		Barrier * m_barrier;
	public:
		Job() = default;

		/** Creates a job without a barrier. */
		inline Job(
			job_fn_t job_fn);

		/** Creates a job connected to a barrier. */
		inline Job(
			job_fn_t job_fn,
			Barrier & barrier);

		/** Exectues the job. */
		void execute();
	};
}

#include "Job.inl"

#endif