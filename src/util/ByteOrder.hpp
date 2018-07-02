#ifndef __crystal_util_byteorder_hpp_defined
#define __crystal_util_byteorder_hpp_defined

#include <cinttypes>
#include <cstddef>

namespace crystal::util
{
	/** The byte order of a computer, compressed into 9 bytes. */
	struct CompressedByteOrder
	{
		// 8 * 3 bits (log2(8) == 3).
		/** The compressed byte order for doubles. */
		std::uint8_t order_double[3];
		/** The compressed byte order for 64 bit integers. */
		std::uint8_t order_64[3];
		// 4 * 2 bits (log2(4) = 2).
		/** The compressed byte order for floats. */
		std::uint8_t order_float[1];
		/** The compressed byte order for 32 bit integers. */
		std::uint8_t order_32[1];
		// 2 * 1 bit (log2(2) = 1).
		/** The compressed byte order for 16 bit integers. */
		std::uint8_t order_16[1];

		/** The size of the structure. */
		static constexpr std::size_t size();
	};

	/** Represents a machine's byte order.
		Can be used to convert values to or from a byte order. */
	class ByteOrder
	{
		/** double byte order. */
		double m_order_double;
		/** 64 bit integer byte order. */
		std::uint64_t m_order_64;
		/** float byte order. */
		float m_order_float;
		/** 32 bit integer byte order. */
		std::uint32_t m_order_32;
		/** 16 bit integer byte order. */
		std::uint16_t m_order_16;

		/** Creates a byte order.
		@param[in] order_double:
			The double byte order.
		@param[in] order_64:
			The 64 bit integer byte order.
		@param[in] order_float:
			The float byte order.
		@param[in] order_32:
			The 32 bit integer byte order.
		@param[in] order_16:
			The 16 bit integer byte order. */
		constexpr ByteOrder(
			double order_double,
			std::uint64_t order_64,
			float order_float,
			std::uint32_t order_32,
			std::uint16_t order_16);
	public:
		ByteOrder() = default;

		/** Parses a byte order from a compressed byte order.
		@param[in] inflate:
			The compressed byte order. */
		ByteOrder(
			CompressedByteOrder const& inflate);

		/** Compresses the byte order for faster transmission.
		@return
			The compressed byte order. */
		CompressedByteOrder compress() const;

		/** The host machine's byte order. */
		static constexpr ByteOrder self();

		/** Little endian byte order. */
		static ByteOrder const kLittleEndian;
		/** Big endian byte order. */
		static ByteOrder const kBigEndian;

		/** Converts a 16 bit integer. */
		inline std::uint16_t convert_16(
			std::uint16_t v) const;
		/** Converts a 32 bit integer. */
		inline std::uint32_t convert_32(
			std::uint32_t v) const;
		/** Converts a 64 bit integer. */
		inline std::uint64_t convert_64(
			std::uint64_t v) const;
		/** Converts a float. */
		inline float convert_float(
			float v) const;
		/** Converts a double. */
		inline double convert_double(
			double v) const;

		/** Validates a byte order.
		@return
			Whether every byte order is valid. */
		bool validate() const;
	};
}

#include "ByteOrder.inl"

#endif