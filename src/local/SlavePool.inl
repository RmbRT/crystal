#include "../self/Paralleliser.hpp"

namespace crystal::local
{
	std::size_t SlavePool::slaves() const
	{
		return m_slaves.size();
	}

	template<class T>
	bool SlavePool::send(
		std::size_t slave,
		T const& data)
	{
		return m_slaves[slave] << data;
	}

	template<class T>
	bool SlavePool::receive(
		std::size_t slave,
		T & data)
	{
		return m_slaves[slave] >> data;
	}

	template<class T>
	bool SlavePool::broadcast(
		T const * data,
		std::size_t count)
	{
		return self::Paralleliser::atomic_reduce_loop(
			0, slaves(),
			1,
			[this, data, count](std::size_t i)->bool {
				for(std::size_t j = 0; j < count; j++)
					if(!(m_slaves[i] << data[j]))
						return false;
				return true;
			},
			true, crystal::self::conjunction);
	}

	template<class T>
	bool SlavePool::gather(
		T * data,
		std::size_t count)
	{
		return self::Paralleliser::atomic_reduce_loop(
			0, slaves(),
			1,
			[this, data, count](std::size_t i)->bool {
				for(std::size_t j = 0; j < count; j++)
					if(!(m_slaves[i] >> data[i*count+j]))
						return false;
				return true;
			},
			true, crystal::self::conjunction);
	}
}