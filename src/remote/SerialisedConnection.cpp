#include "SerialisedConnection.hpp"

namespace crystal::remote
{
	void SerialisedConnection::exchange_byte_order()
	{
		*this << m_send >> m_receive;

		if(!eie::valid(m_receive))
			throw std::runtime_error("Illegal endianness value received.");
	}
}