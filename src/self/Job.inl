namespace crystal::self
{
	Job::Job(
		job_fn_t job_fn):
		m_job_function(job_fn),
		m_barrier(nullptr)
	{
	}

	Job::Job(
		job_fn_t job_fn,
		Barrier & barrier):
		m_job_function(job_fn),
		m_barrier(&barrier)
	{
	}
}