#ifndef __crystal_remote_serialisedconnection_hpp_defined
#define __crystal_remote_serialisedconnection_hpp_defined

#include "../util/Serialiser.hpp"

#include <netlib/x/Connection.hpp>

namespace crystal::remote
{
	/** Serialised connection that allows sending and receiving serialised data. */
	class SerialisedConnection:
		public util::Serialiser,
		public util::Deserialiser,
		private netlib::x::Connection
	{
	public:
		/** Creates a closed connection.
			The byte orders are set to the host byte order. */
		SerialisedConnection() = default;

		SerialisedConnection(
			netlib::x::Connection && connection);

		/** Creates a serialised connection.
		@param[in] send:
			The sending byte order.
		@param[in] receive:
			The receiving byte order.
		@param[in] connection:
			The underlying connection. */
		SerialisedConnection(
			util::ByteOrder const& send,
			util::ByteOrder const& receive,
			netlib::x::Connection && connection);

		using netlib::x::Connection::exists;
		using netlib::x::Connection::connect;
		using netlib::x::Connection::shutdown;
		using netlib::x::Connection::close;
		using netlib::x::Connection::operator bool;

		SerialisedConnection &write(
			void const * data,
			std::size_t size) override;
		SerialisedConnection &read(
			void * data,
			std::size_t size) override;
	};
}

#endif