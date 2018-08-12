namespace crystal::self
{
	bool ExecutionUnit::running() const
	{
		return m_handle.joinable();
	}

	bool ExecutionUnit::busy() const
	{
		return m_busy.load(std::memory_order_relaxed);
	}
}