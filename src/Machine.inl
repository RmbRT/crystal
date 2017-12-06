namespace crystal
{
	Location Machine::location() const
	{
		return m_location;
	}

	util::latency_t Machine::communication() const
	{
		return m_communication;
	}

	util::latency_t Machine::computation_seq() const
	{
		return m_computation_seq;
	}

	util::latency_t Machine::computation_par() const
	{
		return m_computation_par;
	}
}