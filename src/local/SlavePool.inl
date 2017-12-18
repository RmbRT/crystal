namespace crystal::local
{
	std::size_t SlavePool::slaves() const
	{
		return m_slaves.size();
	}

	util::ByteOrder const& SlavePool::byte_order(
		std::size_t slave) const
	{
		return m_slaves[slave].byte_order();
	}

	bool SlavePool::send(
		std::size_t slave,
		void const * data,
		std::size_t size)
	{
		return m_slaves[slave].send(data, size);
	}

	bool SlavePool::receive(
		std::size_t slave,
		void * data,
		std::size_t size)
	{
		return m_slaves[slave].receive(data, size);
	}
}