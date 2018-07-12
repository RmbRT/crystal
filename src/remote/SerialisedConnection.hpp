#ifndef __crystal_remote_serialisedconnection_hpp_defined
#define __crystal_remote_serialisedconnection_hpp_defined

#include "../util/Serialiser.hpp"
#include "../util/Deserialiser.hpp"

#include <netlib/x/Connection.hpp>

namespace crystal::remote
{
	/** Serialised connection that allows sending and receiving serialised data. */
	class SerialisedConnection:
		private netlib::x::Connection,
		private util::SerialiserUnion<SerialisedConnection>,
		private util::DeserialiserUnion<SerialisedConnection>
	{
	protected:
		util::Endian m_send;
		util::Endian m_receive;
	public:
		SerialisedConnection(
			netlib::x::Connection && connection,
			util::Endian send,
			util::Endian receive);

		SerialisedConnection(
			netlib::x::Connection && connection,
			util::Endian send = util::endian::kSelf);

		// netlib::x::Connection.
		using netlib::x::Connection::Connection;

		using netlib::x::Connection::exists;
		using netlib::x::Connection::connect;
		using netlib::x::Connection::shutdown;
		using netlib::x::Connection::close;
		using netlib::x::Connection::operator bool;
		using netlib::x::Connection::pending;

		// Serialiser and Deserialiser.
		using SerialiserUnion<SerialisedConnection>::LittleSerialiser;
		using SerialiserUnion<SerialisedConnection>::BigSerialiser;
		using DeserialiserUnion<SerialisedConnection>::LittleDeserialiser;
		using DeserialiserUnion<SerialisedConnection>::BigDeserialiser;

		inline LittleSerialiser &little_serialiser();
		inline BigSerialiser &big_serialiser();

		inline LittleDeserialiser &little_deserialiser();
		inline BigDeserialiser &big_deserialiser();

		template<class T>
		inline SerialisedConnection &operator<<(
			T const& value);

		template<class T>
		inline SerialisedConnection &operator>>(
			T &value);

		static constexpr SerialisedConnection &base(
			SerialiserUnion<SerialisedConnection> &serialiser);
		static constexpr SerialisedConnection &base(
			DeserialiserUnion<SerialisedConnection> &deserialiser);

		inline void serialise(
			void const * data,
			std::size_t size);
		inline void deserialise(
			void * data,
			std::size_t size);
	private:
		/** Sends the host machine's byte order and receives the other machine's byte order. */
		void exchange_byte_order();
	};
}

#include "SerialisedConnection.inl"

#endif