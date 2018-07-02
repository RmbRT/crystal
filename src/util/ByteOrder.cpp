#include "ByteOrder.hpp"
#include <cstring>

namespace crystal::util
{
	ByteOrder const ByteOrder::kLittleEndian {
		CompressedByteOrder {
			{0x88, 0xc6, 0xfa},
			{0x88, 0xc6, 0xfa},
			{0xe4},
			{0xe4},
			{0x02}
		}
	};

	ByteOrder const ByteOrder::kBigEndian {
		CompressedByteOrder {
			{0x77, 0x39, 0x05},
			{0x77, 0x39, 0x05},
			{0x1b},
			{0x1b},
			{0x01}
		}
	};

	inline void set_bit(
		void * bytes,
		std::size_t bit,
		bool value)
	{
		if(value)
			reinterpret_cast<std::uint8_t *>(bytes)[bit>>3] |= 1<<(bit&7);
		else
			reinterpret_cast<std::uint8_t *>(bytes)[bit>>3] &= ~(1<<(bit&7));
	}

	inline bool get_bit(
		void const * bytes,
		std::size_t bit)
	{
		return reinterpret_cast<std::uint8_t const *>(bytes)[bit>>3] & (1<<(bit&7));
	}

	static inline void inflate64(
		std::uint8_t const * in,
		void * out)
	{
		std::memset(out, 0, 8);

		for(std::size_t group = 0; group < 8; group++)
		{
			set_bit(out, 8*group, get_bit(in, 3*group));
			set_bit(out, 8*group + 1, get_bit(in, 3*group + 1));
			set_bit(out, 8*group + 2, get_bit(in, 3*group + 2));
		}
	}

	static inline void inflate32(
		std::uint8_t const * in,
		void * out)
	{
		std::memset(out, 0, 4);
		for(std::size_t group = 0; group < 4; group++)
		{
			set_bit(out, 8*group, get_bit(in, 2 * group));
			set_bit(out, 8*group + 1, get_bit(in, 2 * group + 1));
		}
	}

	static inline void inflate16(
		std::uint8_t const * in,
		void * out)
	{
		std::memset(out, 0, 2);
		for(std::size_t group = 0; group < 2; group++)
			set_bit(out, 8*group, get_bit(in, group));
	}

	ByteOrder::ByteOrder(
		CompressedByteOrder const& inflate)
	{
		inflate64(inflate.order_double, &m_order_double);
		inflate64(inflate.order_64, &m_order_64);

		inflate32(inflate.order_float, &m_order_float);
		inflate32(inflate.order_32, &m_order_32);

		inflate16(inflate.order_16, &m_order_16);
	}

	static inline void deflate64(
		void const * in,
		std::uint8_t * out)
	{
		for(std::size_t group = 0; group < 8; group++)
		{
			set_bit(out, 3*group, get_bit(in, 8*group));
			set_bit(out, 3*group + 1, get_bit(in, 8*group + 1));
			set_bit(out, 3*group + 2, get_bit(in, 8*group + 2));
		}
	}

	static inline void deflate32(
		void const * in,
		std::uint8_t * out)
	{
		for(std::size_t group = 0; group < 4; group++)
		{
			set_bit(out, 2*group, get_bit(in, 8*group));
			set_bit(out, 2*group + 1, get_bit(in, 8*group + 1));
		}
	}

	static inline void deflate16(
		void const * in,
		std::uint8_t * out)
	{
		for(std::size_t group = 0; group < 2; group++)
			set_bit(out, group, get_bit(in, 8*group));
	}

	CompressedByteOrder ByteOrder::compress() const
	{
		CompressedByteOrder out;
		deflate64(&m_order_double, out.order_double);
		deflate64(&m_order_64, out.order_64);

		deflate32(&m_order_float, out.order_float);
		deflate32(&m_order_32, out.order_32);

		deflate16(&m_order_16, out.order_16);

		return out;
	}

	static inline bool validate(
		void const * v,
		std::size_t n)
	{
		std::uint8_t const * in = reinterpret_cast<std::uint8_t const *>(v);

		std::uint8_t flags = (1 << n) - 1;

		for(std::size_t i = 0; i < n; i++)
		{
			if(in[i] >= n
			|| !(flags & (1<<in[i])))
				return false;

			flags &= ~(1<<in[i]);
		}

		return flags == 0;
	}

	bool ByteOrder::validate() const
	{
		return util::validate(&m_order_double, 8)
			&& util::validate(&m_order_64, 8)
			&& util::validate(&m_order_float, 4)
			&& util::validate(&m_order_32, 4)
			&& util::validate(&m_order_16, 2);
	}
}