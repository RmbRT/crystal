namespace crystal::util
{
	constexpr std::size_t CompressedByteOrder::size()
	{
		return 9;
	}
	constexpr ByteOrder::ByteOrder(
			double order_double,
			std::uint64_t order_64,
			float order_float,
			std::uint32_t order_32,
			std::uint16_t order_16):
		m_order_double(order_double),
		m_order_64(order_64),
		m_order_float(order_float),
		m_order_32(order_32),
		m_order_16(order_16)
	{
	}

	constexpr ByteOrder ByteOrder::self()
	{
		return ByteOrder(
			0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000794992889512736253615566268553285340675338476556009692851631683095909725116229155007651506655011549819762651632490357850862619156912209691539383912386080152959007607165151845581748485352320987726499064146480118850009296246762535443466064333054562507972277777016152888533100483211597371351756917973939996730211835272977340454396316067101147446443224925142575972155309831163829325614052576156662841471257219688597206542432913798337105697228825649818615802998357281772873275667957953954230630363647261099462833213291010999020083818596436209242021586876817073591466975543876822788878454918453089230931334888899046711833077895764542436555526627901813174759126923163421452045440673828125,
			0x0706050403020100,
			0.0000000000000000000000000000000000003820471434542631889912843817899761562840137538567562969075465384438672344913356937468051910400390625f,
			0x03020100,
			0x0100);
	}

	std::uint16_t ByteOrder::convert_16(
		std::uint16_t v) const
	{
		if(m_order_16 == self().m_order_16)
			return v;
		else
		{
			std::uint16_t out;

			for(std::size_t i = 0; i < sizeof(v); i++)
			{
				reinterpret_cast<std::uint8_t*>(&out)[
					reinterpret_cast<std::uint8_t const*>(&m_order_16)[i]] =
					reinterpret_cast<std::uint8_t*>(&v)[i];
			}
		}
	}

	std::uint32_t ByteOrder::convert_32(
		std::uint32_t v) const
	{
		if(m_order_32 == self().m_order_32)
			return v;
		else
		{
			std::uint32_t out;

			for(std::size_t i = 0; i < sizeof(v); i++)
			{
				reinterpret_cast<std::uint8_t*>(&out)[
					reinterpret_cast<std::uint8_t const*>(&m_order_32)[i]] =
					reinterpret_cast<std::uint8_t*>(&v)[i];
			}
		}
	}

	std::uint64_t ByteOrder::convert_64(
		std::uint64_t v) const
	{
		if(m_order_64 == self().m_order_64)
			return v;
		else
		{
			std::uint64_t out;

			for(std::size_t i = 0; i < sizeof(v); i++)
			{
				reinterpret_cast<std::uint8_t*>(&out)[
					reinterpret_cast<std::uint8_t const*>(&m_order_64)[i]] =
					reinterpret_cast<std::uint8_t*>(&v)[i];
			}

			return out;
		}
	}

	float ByteOrder::convert_float(
		float v) const
	{
		if(m_order_float == self().m_order_float)
			return v;
		else
		{
			float out;

			for(std::size_t i = 0; i < sizeof(v); i++)
			{
				reinterpret_cast<std::uint8_t*>(&out)[
					reinterpret_cast<std::uint8_t const*>(&m_order_float)[i]] =
					reinterpret_cast<std::uint8_t*>(&v)[i];
			}
		}
	}

	double ByteOrder::convert_double(
		double v) const
	{
		if(m_order_double == self().m_order_double)
			return v;
		else
		{
			double out;

			for(std::size_t i = 0; i < sizeof(v); i++)
			{
				reinterpret_cast<std::uint8_t*>(&out)[
					reinterpret_cast<std::uint8_t const*>(&m_order_double)[i]] =
					reinterpret_cast<std::uint8_t*>(&v)[i];
			}
		}
	}
}