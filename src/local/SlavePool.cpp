#include "SlavePool.hpp"

namespace crystal::local
{
	bool SlavePool::accept()
	{
		if(remote::SerialisedConnection conn = netlib::x::ConnectionListener::accept())
		{
			m_slaves.emplace_back(std::move(conn));
			return true;
		} else
			return false;
	}
}