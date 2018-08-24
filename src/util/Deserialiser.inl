namespace crystal::util
{
	template<Endian kEndian, class T>
	Deserialiser<kEndian, T> &Deserialiser<kEndian, T>::operator>>(
		std::uint8_t &v)
	{
		T::base(*this).deserialise(&v, 1);
		return *this;
	}

	template<Endian kEndian, class T>
	Deserialiser<kEndian, T> &Deserialiser<kEndian, T>::operator>>(
		std::uint16_t &v)
	{
		T::base(*this).deserialise(&v, 2);
		v = endian::convert_16<kEndian>(v);
		return *this;
	}

	template<Endian kEndian, class T>
	Deserialiser<kEndian, T> &Deserialiser<kEndian, T>::operator>>(
		std::uint32_t &v)
	{
		T::base(*this).deserialise(&v, 4);
		v = endian::convert_32<kEndian>(v);
		return *this;
	}

	template<Endian kEndian, class T>
	Deserialiser<kEndian, T> &Deserialiser<kEndian, T>::operator>>(
		std::uint64_t &v)
	{
		T::base(*this).deserialise(&v, 8);
		v = endian::convert_64<kEndian>(v);
		return *this;
	}

	template<Endian kEndian, class T>
	Deserialiser<kEndian, T> &Deserialiser<kEndian, T>::operator>>(
		float &v)
	{
		T::base(*this).deserialise(&v, 4);
		v = endian::convert_float<kEndian>(v);
		return *this;
	}

	template<Endian kEndian, class T>
	Deserialiser<kEndian, T> &Deserialiser<kEndian, T>::operator>>(
		double &v)
	{
		T::base(*this).deserialise(&v, 8);
		v = endian::convert_double<kEndian>(v);
		return *this;
	}

	template<class T, Endian kEndian>
	void ReceivePrimitive<T, kEndian>::prepare(
		netlib::x::BufferedConnection &connection,
		T &data)
	{
		LibCrBase::prepare();
		this->data = &data;
		receive.prepare(
			connection,
			&data,
			sizeof(T));
	}

	template<class T, Endian kEndian>
	void ReceivePrimitive<T, kEndian>::prepare(
		netlib::x::BufferedConnection &connection,
		T &data,
		cr::Coroutine * parent)
	{
		LibCrBase::prepare(parent);
		this->data = &data;
		this->connection = connection;
	}

	template<class T, Endian kEndian>
	CR_IMPL(ReceivePrimitive<T, kEndian>)
		CR_CALL(receive,
			*connection,
			data,
			sizeof(T));

		if(kEndian != endian::kSelf)
			*data = endian::convert<kEndian>(*data);
	CR_IMPL_END

	template<class T>
	constexpr Deserialiser<Endian::kLittle, DeserialiserUnion<T>> &DeserialiserUnion<T>::little_deserialiser()
	{
		return *static_cast<LittleDeserialiser *>(this);
	}

	template<class T>
	constexpr Deserialiser<Endian::kBig, DeserialiserUnion<T>> &DeserialiserUnion<T>::big_deserialiser()
	{
		return *static_cast<BigDeserialiser *>(this);
	}

	template<class T>
	constexpr DeserialiserUnion<T> &DeserialiserUnion<T>::base(
		Deserialiser<Endian::kLittle, DeserialiserUnion<T>> &serialiser)
	{
		return static_cast<DeserialiserUnion<T> &>(serialiser);
	}

	template<class T>
	constexpr DeserialiserUnion<T> &DeserialiserUnion<T>::base(
		Deserialiser<Endian::kBig, DeserialiserUnion<T>> &serialiser)
	{
		return static_cast<DeserialiserUnion<T> &>(serialiser);
	}

	template<class T>
	void DeserialiserUnion<T>::deserialise(
		void * data,
		std::size_t size)
	{
		T::base(*this).deserialise(data, size);
	}

	template<Endian kEndian, class T>
	Deserialiser<kEndian, T> &operator>>(
		Deserialiser<kEndian, T> &s,
		Endian &v)
	{
		std::uint8_t x;
		s >> x;
		v = Endian(x);
		return s;
	}
}