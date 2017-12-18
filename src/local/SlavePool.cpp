#include "SlavePool.hpp"
#include "../self/Paralleliser.hpp"

namespace crystal::local
{
	Machine SlavePool::accept()
	{
		return Machine(
			netlib::x::ConnectionListener::accept());
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