namespace crystal::remote
{
	SerialisedConnection::LittleSerialiser &SerialisedConnection::little_serialiser()
	{
		assert(m_send == util::Endian::kLittle);
		return SerialiserUnion<SerialisedConnection>::little_serialiser();
	}
	SerialisedConnection::BigSerialiser &SerialisedConnection::big_serialiser()
	{
		assert(m_send == util::Endian::kBig);
	}

	SerialisedConnection::LittleDeserialiser &SerialisedConnection::little_deserialiser()
	{
		assert(m_send == util::Endian::kLittle);
		return DeserialiserUnion<SerialisedConnection>::little_deserialiser();
	}
	SerialisedConnection::BigDeserialiser &SerialisedConnection::big_deserialiser()
	{
		assert(m_send == util::Endian::kBig);
		return DeserialiserUnion<SerialisedConnection>::big_deserialiser();
	}

	template<class T>
	SerialisedConnection &SerialisedConnection::operator<<(
		T const& value)
	{
		if(m_send == util::Endian::kLittle)
			little_serialiser() << value;
		else
			big_serialiser() << value;

		return *this;
	}

	template<class T>
	SerialisedConnection &SerialisedConnection::operator>>(
		T & value)
	{
		if(m_receive == util::Endian::kLittle)
			little_deserialiser() >> value;
		else
			big_deserialiser() >> value;

		return *this;
	}

	constexpr SerialisedConnection & SerialisedConnection::base(
		SerialiserUnion<SerialisedConnection> &serialiser)
	{
		return static_cast<SerialisedConnection &>(serialiser);
	}

	constexpr SerialisedConnection & SerialisedConnection::base(
		DeserialiserUnion<SerialisedConnection> &deserialiser)
	{
		return static_cast<SerialisedConnection &>(deserialiser);
	}

	void SerialisedConnection::serialise(
		void const * data,
		std::size_t size)
	{
		if(!send(data, size))
			throw std::runtime_error("Failed to send data.");
	}

	void SerialisedConnection::deserialise(
		void * data,
		std::size_t size)
	{
		if(!receive(data, size))
			throw std::runtime_error("Failed to receive data.");
	}


}