#include "Machine.hpp"
#include <stdexcept>
#include <cassert>

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
		if(!send_byte_order())
			throw std::runtime_error("Failed to send byte order.");
		else if(!receive_byte_order())
			throw std::runtime_error("Failed to receive byte order.");
	}

	Machine::Machine(
		Location location,
		netlib::SocketAddress const& address):
		crystal::Machine(
			location),
		netlib::x::Connection(netlib::StreamSocket(address.family))
	{
		if(!netlib::x::Connection::connect(address))
			throw std::runtime_error("Connection failed.");
		if(!receive_byte_order())
			throw std::runtime_error("Failed to receive byte order.");
		else if(!send_byte_order())
			throw std::runtime_error("Failed to send byte order.");
	}

	Machine::~Machine() = default;

	bool Machine::send_byte_order()
	{
		util::CompressedByteOrder compressed = util::ByteOrder::self().compress();
		return send(&compressed, util::CompressedByteOrder::size());
	}

	bool Machine::receive_byte_order()
	{
		util::CompressedByteOrder compressed;
		if(receive(&compressed, util::CompressedByteOrder::size()))
		{
			m_byte_order = compressed;
			assert(m_byte_order.validate());
			return true;
		} else
			return false;
	}
}