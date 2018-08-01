namespace crystal::util
{

	constexpr OpenMode operator|(OpenMode a, OpenMode b)
	{
		return OpenMode(std::uint8_t(a) | std::uint8_t(b));
	}
	constexpr bool operator&(OpenMode a, OpenMode b)
	{
		return std::uint8_t(a) & std::uint8_t(b);

	}

	template<class T>
	SerialisedFile &SerialisedFile::operator<<(
		T const& value)
	{
		assert(eie::valid(m_endian));

		if(m_endian == Endian::kLittle)
			little_serialiser() << value;
		else
			big_serialiser() << value;

		return *this;
	}

	template<class T>
	SerialisedFile &SerialisedFile::operator>>(
		T &value)
	{
		assert(eie::valid(m_endian));

		if(m_endian == Endian::kLittle)
			little_deserialiser() >> value;
		else
			big_deserialiser() >> value;

		return *this;
	}

	template<class T>
	void SerialisedFile::write_formatted(
		T const& v)
	{
		std::fstream::operator<<(v);
	}

	template<class T>
	void SerialisedFile::read_formatted(
		T & v)
	{
		std::fstream::operator>>(v);
	}

	void SerialisedFile::write(
		void const * data,
		std::size_t size)
	{
		std::fstream::write(reinterpret_cast<char const *>(data), size);
	}

	void SerialisedFile::read(
		void * data,
		std::size_t size)
	{
		std::fstream::read(reinterpret_cast<char *>(data), size);
	}

	SerialisedFile::LittleSerialiser &SerialisedFile::little_serialiser()
	{
		assert(m_endian == util::Endian::kLittle);
		return SerialiserUnion<SerialisedFile>::little_serialiser();
	}
	SerialisedFile::BigSerialiser &SerialisedFile::big_serialiser()
	{
		assert(m_endian == util::Endian::kBig);
		return SerialiserUnion<SerialisedFile>::big_serialiser();
	}

	SerialisedFile::LittleDeserialiser &SerialisedFile::little_deserialiser()
	{
		assert(m_endian == util::Endian::kLittle);
		return DeserialiserUnion<SerialisedFile>::little_deserialiser();
	}
	SerialisedFile::BigDeserialiser &SerialisedFile::big_deserialiser()
	{
		assert(m_endian == util::Endian::kBig);
		return DeserialiserUnion<SerialisedFile>::big_deserialiser();
	}

	constexpr SerialisedFile & SerialisedFile::base(
		SerialiserUnion<SerialisedFile> &serialiser)
	{
		return static_cast<SerialisedFile &>(serialiser);
	}

	constexpr SerialisedFile & SerialisedFile::base(
		DeserialiserUnion<SerialisedFile> &deserialiser)
	{
		return static_cast<SerialisedFile &>(deserialiser);
	}

	void SerialisedFile::serialise(
		void const * data,
		std::size_t size)
	{
		write(data, size);
	}

	void SerialisedFile::deserialise(
		void * data,
		std::size_t size)
	{
		read(data, size);
	}
}