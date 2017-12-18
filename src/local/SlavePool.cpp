#include "SlavePool.hpp"
#include "../self/Paralleliser.hpp"

namespace crystal::local
{
	bool SlavePool::accept()
	{
		if(netlib::x::Connection conn = netlib::x::ConnectionListener::accept())
		{
			m_slaves.emplace_back(std::move(conn));
			return true;
		} else
			return false;
	}

	bool SlavePool::broadcast(
		void const * data,
		std::size_t size)
	{
		return self::Paralleliser::atomic_reduce_loop(
			0, slaves(),
			1,
			[this, data, size](std::size_t i)->bool {
				return m_slaves[i].send(data, size);
			},
			true, crystal::self::conjunction);
	}

	bool SlavePool::gather(
		void * data,
		std::size_t size)
	{
		return self::Paralleliser::atomic_reduce_loop(
			0, slaves(),
			1,
			[this, data, size](std::size_t i)->bool {
				return m_slaves[i].receive(
					reinterpret_cast<std::uint8_t*>(data)+i*size,
					size);
			},
			true, crystal::self::conjunction);
	}
}