namespace crystal::util
{
	template<Endian kEndian, class T>
	Serialiser<kEndian, T> &Serialiser<kEndian, T>::operator<<(
		std::uint8_t v)
	{
		T::base(*this).serialise(&v, 1);
		return *this;
	}

	template<Endian kEndian, class T>
	Serialiser<kEndian, T> &Serialiser<kEndian, T>::operator<<(
		std::uint16_t v)
	{
		v = endian::convert_16<kEndian>(v);
		T::base(*this).serialise(&v, 2);
		return *this;
	}

	template<Endian kEndian, class T>
	Serialiser<kEndian, T> &Serialiser<kEndian, T>::operator<<(
		std::uint32_t v)
	{
		v = endian::convert_32<kEndian>(v);
		T::base(*this).serialise(&v, 4);
		return *this;
	}

	template<Endian kEndian, class T>
	Serialiser<kEndian, T> &Serialiser<kEndian, T>::operator<<(
		std::uint64_t v)
	{
		v = endian::convert_64<kEndian>(v);
		T::base(*this).serialise(&v, 8);
		return *this;
	}

	template<Endian kEndian, class T>
	Serialiser<kEndian, T> &Serialiser<kEndian, T>::operator<<(
		float v)
	{
		v = endian::convert_float<kEndian>(v);
		T::base(*this).serialise(&v, 4);
		return *this;
	}

	template<Endian kEndian, class T>
	Serialiser<kEndian, T> &Serialiser<kEndian, T>::operator<<(
		double v)
	{
		v = endian::convert_double<kEndian>(v);
		T::base(*this).serialise(&v, 8);
		return *this;
	}

	template<class T>
	constexpr Serialiser<Endian::kLittle, SerialiserUnion<T>> &SerialiserUnion<T>::little_serialiser()
	{
		return *static_cast<LittleSerialiser *>(this);
	}

	template<class T>
	constexpr Serialiser<Endian::kBig, SerialiserUnion<T>> &SerialiserUnion<T>::big_serialiser()
	{
		return *static_cast<BigSerialiser *>(this);
	}

	template<class T, Endian kEndian>
	void SendPrimitive<T, kEndian>::prepare(
		netlib::x::BufferedConnection &connection,
		T data)
	{
		this->data = endian::convert<kEndian>(data);
		netlib::async::BufferedSend::prepare(
			connection,
			&data,
			sizeof(T));
	}

	template<class T, Endian kEndian>
	void SendPrimitive<T, kEndian>::prepare(
		netlib::x::BufferedConnection &connection,
		T data,
		cr::Coroutine * parent)
	{
		this->data = endian::convert<kEndian>(data);
		netlib::async::BufferedSend::prepare(
			connection,
			&data,
			sizeof(T),
			parent);
	}

	template<class T>
	constexpr SerialiserUnion<T> &SerialiserUnion<T>::base(
		Serialiser<Endian::kLittle, SerialiserUnion<T>> &serialiser)
	{
		return *static_cast<SerialiserUnion<T>*>(&serialiser);
	}

	template<class T>
	constexpr SerialiserUnion<T> &SerialiserUnion<T>::base(
		Serialiser<Endian::kBig, SerialiserUnion<T>> &serialiser)
	{
		return *static_cast<SerialiserUnion<T>*>(&serialiser);
	}

	template<class T>
	void SerialiserUnion<T>::serialise(
		void const * data,
		std::size_t size)
	{
		return T::base(*this).serialise(data, size);
	}

	template<Endian kEndian, class T>
	Serialiser<kEndian, T> &operator<<(
		Serialiser<kEndian, T> &s,
		Endian v)
	{
		return s << std::uint8_t(v);
	}
}