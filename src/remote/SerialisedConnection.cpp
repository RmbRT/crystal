#include "SerialisedConnection.hpp"

namespace crystal::remote
{
	SerialisedConnection::SerialisedConnection(
		util::ByteOrder const& send,
		util::ByteOrder const& receive,
		netlib::x::Connection && connection):
		util::Serialiser(send),
		util::Deserialiser(receive),
		netlib::x::Connection(std::move(connection))
	{
	}

	SerialisedConnection &SerialisedConnection::write(
		void const * data,
		std::size_t size)
	{
		if(!send(data, size))
			Serialiser::m_good = false;

		return *this;
	}

	SerialisedConnection &SerialisedConnection::read(
		void * data,
		std::size_t size)
	{
		if(!receive(data, size))
			Deserialiser::m_good = false;

		return *this;
	}
}