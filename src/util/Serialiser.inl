namespace crystal::util
{
	Serialiser &Serialiser::operator<<(
		std::uint8_t v)
	{
		write(&v, 1);
		return *this;
	}

	Serialiser &Serialiser::operator<<(
		std::uint16_t v)
	{
		v = m_byte_order.convert_16(v);
		write(&v, 2);
		return *this;
	}

	Serialiser &Serialiser::operator<<(
		std::uint32_t v)
	{
		v = m_byte_order.convert_32(v);
		write(&v, 4);
		return *this;
	}

	Serialiser &Serialiser::operator<<(
		std::uint64_t v)
	{
		v = m_byte_order.convert_64(v);
		write(&v, 8);
		return *this;
	}

	Serialiser &Serialiser::operator<<(
		float v)
	{
		v = m_byte_order.convert_float(v);
		write(&v, 4);
		return *this;
	}

	Serialiser &Serialiser::operator<<(
		double v)
	{
		v = m_byte_order.convert_double(v);
		write(&v, 8);
		return *this;
	}

	Deserialiser &Deserialiser::operator>>(
		std::uint8_t &v)
	{
		read(&v, 1);
		return *this;
	}

	Deserialiser &Deserialiser::operator>>(
		std::uint16_t &v)
	{
		read(&v, 2);
		v = m_byte_order.convert_16(v);
		return *this;
	}

	Deserialiser &Deserialiser::operator>>(
		std::uint32_t &v)
	{
		read(&v, 4);
		v = m_byte_order.convert_32(v);
		return *this;
	}

	Deserialiser &Deserialiser::operator>>(
		std::uint64_t &v)
	{
		read(&v, 8);
		v = m_byte_order.convert_64(v);
		return *this;
	}

	Deserialiser &Deserialiser::operator>>(
		float &v)
	{
		read(&v, 4);
		v = m_byte_order.convert_float(v);
		return *this;
	}

	Deserialiser &Deserialiser::operator>>(
		double &v)
	{
		read(&v, 8);
		v = m_byte_order.convert_double(v);
		return *this;
	}
}