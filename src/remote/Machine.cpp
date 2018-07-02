#include "Machine.hpp"
#include <stdexcept>
#include <cassert>

namespace crystal::remote
{
	Machine::Machine(
		Location location,
		SerialisedConnection && connection):
		crystal::Machine(
			location),
		SerialisedConnection(std::move(connection))
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
		SerialisedConnection(netlib::StreamSocket(address.family))
	{
		if(!SerialisedConnection::connect(address))
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
		return write(&compressed, util::CompressedByteOrder::size());
	}

	bool Machine::receive_byte_order()
	{
		util::CompressedByteOrder compressed;
		if(read(&compressed, util::CompressedByteOrder::size()))
		{
			util::ByteOrder inflate { compressed };
			assert(inflate.validate());

			set_target_order(inflate);
			return true;
		} else
			return false;
	}
}