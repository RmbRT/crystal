#include "Machine.hpp"
#include <stdexcept>

namespace crystal::remote
{
	Machine::Machine(
		Location location,
		netlib::x::Connection && connection):
		crystal::Machine(
			location),
		netlib::x::Connection(std::move(connection)),
		m_byte_order()
	{
		if(!send_byte_order() || !receive_byte_order())
			throw std::runtime_error("Failed to exchange byte order.");
	}

	Machine::Machine(
		Location location,
		netlib::SocketAddress const& address):
		crystal::Machine(
			location),
		netlib::x::Connection()
	{
		if(!netlib::x::Connection::connect(address))
			throw std::runtime_error("Connection failed.");
		if(!receive_byte_order() || !send_byte_order())
			throw std::runtime_error("Failed to exchange byte order.");
	}

	bool Machine::send_byte_order()
	{
		util::CompressedByteOrder compressed = util::ByteOrder::self().compress();
		return send(&compressed, sizeof(util::CompressedByteOrder));
	}

	bool Machine::receive_byte_order()
	{
		util::CompressedByteOrder compressed;
		if(receive(&compressed, sizeof(util::CompressedByteOrder)))
		{
			m_byte_order = compressed;
			return true;
		} else
			return false;
	}
}