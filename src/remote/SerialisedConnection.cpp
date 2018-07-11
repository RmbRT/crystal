#include "SerialisedConnection.hpp"

namespace crystal::remote
{
	SerialisedConnection::SerialisedConnection(
		netlib::x::Connection && connection,
		util::Endian send,
		util::Endian receive):
		util::SerialiserUnion<SerialisedConnection>(),
		util::DeserialiserUnion<SerialisedConnection>(),
		netlib::x::Connection(std::move(connection)),
		m_send(send),
		m_receive(receive)
	{
	}

	SerialisedConnection::SerialisedConnection(
		netlib::x::Connection && connection,
		util::Endian send):
		util::SerialiserUnion<SerialisedConnection>(),
		util::DeserialiserUnion<SerialisedConnection>(),
		netlib::x::Connection(std::move(connection)),
		m_send(send)
	{
		exchange_byte_order();
	}

	void SerialisedConnection::exchange_byte_order()
	{
		*this << m_send;

		deserialise(&m_receive, sizeof(util::Endian));

		if(!eie::valid(m_receive))
			throw std::runtime_error("Illegal endianness value received.");
	}
}