#include "Job.hpp"

namespace crystal::local
{
	void Job::execute()
	{
		m_job_function();
		if(m_barrier)
			m_barrier->notify();
	}
}