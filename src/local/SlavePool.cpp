#include "SlavePool.hpp"

namespace crystal::local
{
	bool SlavePool::accept()
	{
		if(auto conn = netlib::x::ConnectionListener::accept())
		{
			m_slaves.emplace_back(
				remote::SerialisedConnection(
					std::move(conn)));

			return true;
		} else
			return false;
	}
}