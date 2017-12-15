#include "ByteOrder.hpp"
#include <bitset>

namespace crystal::util
{
	static inline void inflate64(
		std::uint8_t const * in,
		void * out)
	{
		std::bitset<24> const * _in = reinterpret_cast<std::bitset<24> const *>(in);
		std::bitset<64> * _out = static_cast<std::bitset<64> *>(out);
		_out->reset();

		for(std::size_t group = 0; group < 8; group++)
		{
			_out->set(8*group, _in->test(3 * group));
			_out->set(8*group + 1, _in->test(3 * group + 1));
			_out->set(8*group + 2, _in->test(3 * group + 2));
		}
	}

	static inline void inflate32(
		std::uint8_t const * in,
		void * out)
	{
		std::bitset<8> const * _in = reinterpret_cast<std::bitset<8> const *>(in);
		std::bitset<32> * _out = static_cast<std::bitset<32> *>(out);
		_out->reset();

		for(std::size_t group = 0; group < 4; group++)
		{
			_out->set(8*group, _in->test(2 * group));
			_out->set(8*group + 1, _in->test(2 * group + 1));
		}
	}

	static inline void inflate16(
		std::uint8_t const * in,
		void * out)
	{
		std::bitset<2> const * _in = reinterpret_cast<std::bitset<2> const *>(in);
		std::bitset<16> * _out = static_cast<std::bitset<16> *>(out);
		_out->reset();

		for(std::size_t group = 0; group < 2; group++)
			_out->set(8*group, _in->test(group));
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
		std::bitset<64> const * _in = reinterpret_cast<std::bitset<64> const *>(in);
		std::bitset<24> * _out = reinterpret_cast<std::bitset<24> *>(out);

		for(std::size_t group = 0; group < 8; group++)
		{
			_out->set(3*group, _in->test(8*group));
			_out->set(3*group + 1, _in->test(8*group + 1));
			_out->set(3*group + 2, _in->test(8*group + 2));
		}
	}

	static inline void deflate32(
		void const * in,
		std::uint8_t * out)
	{
		std::bitset<32> const * _in = reinterpret_cast<std::bitset<32> const *>(in);
		std::bitset<8> * _out = reinterpret_cast<std::bitset<8> *>(out);

		for(std::size_t group = 0; group < 4; group++)
		{
			_out->set(2*group, _in->test(8*group));
			_out->set(2*group + 1, _in->test(8*group + 1));
		}
	}

	static inline void deflate16(
		void const * in,
		std::uint8_t * out)
	{
		std::bitset<16> const * _in = reinterpret_cast<std::bitset<16> const *>(in);
		std::bitset<2> * _out = reinterpret_cast<std::bitset<2> *>(out);

		for(std::size_t group = 0; group < 2; group++)
			_out->set(group, _in->test(8*group));
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
}