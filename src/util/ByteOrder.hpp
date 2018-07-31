#ifndef __crystal_util_byteorder_hpp_defined
#define __crystal_util_byteorder_hpp_defined

#include <cinttypes>
#include <cstddef>

#include <cppeie/eie.hpp>

namespace crystal::util
{
	/** Endianness values. */
	enum class Endian : std::uint8_t
	{
		/** Little endian. */
		kLittle,
		/** Big endian. */
	EIE(kBig)
	};
}

namespace crystal::util::endian
{
#if defined(__BYTE_ORDER) && (__BYTE_ORDER == __BIG_ENDIAN) || \
	defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) || \
	defined(__BIG_ENDIAN__) || \
	defined(__ARMEB__) || \
	defined(__THUMBEB__) || \
	defined(__AARCH64EB__) || \
	defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
	constexpr Endian kSelf = Endian::kBig;
#define ENDIANCVT constexpr
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
	defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
	defined(__LITTLE_ENDIAN__) || \
	defined(__ARMEL__) || \
	defined(__THUMBEL__) || \
	defined(__AARCH64EL__) || \
	defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
	constexpr Endian kSelf = Endian::kLittle;
#define ENDIANCVT constexpr
#else
	namespace detail
	{
		static constexpr union
		{
			std::uint16_t check;
			std::uint8_t result[2];
		} endianness { 1 };
	}

	static Endian const kSelf = detail::endianness.result[0] == 1
		? Endian::kLittle
			: Endian::kBig;
#define ENDIANCVT inline
#endif


	constexpr std::uint16_t reverse_16(
		std::uint16_t v);
	constexpr std::uint32_t reverse_32(
		std::uint32_t v);
	constexpr std::uint64_t reverse_64(
		std::uint64_t v);
	constexpr float reverse_float(
		float v);
	constexpr float reverse_double(
		double v);

	template<class T>
	constexpr T reverse(
		T v);

	template<Endian endian>
	ENDIANCVT std::uint16_t convert_16(
		std::uint16_t v);

	template<Endian endian>
	ENDIANCVT std::uint32_t convert_32(
		std::uint32_t v);

	template<Endian endian>
	ENDIANCVT std::uint64_t convert_64(
		std::uint64_t v);

	template<Endian endian>
	ENDIANCVT float convert_float(
		float v);

	template<Endian endian>
	ENDIANCVT double convert_double(
		double d);

	template<Endian endian, class T>
	inline T convert(
		T const& v);

	template<Endian endian>
	inline void convert_16(
		std::uint16_t const * in,
		std::uint16_t * out,
		std::size_t size);

	template<Endian endian>
	inline void convert_32(
		std::uint32_t const * in,
		std::uint32_t * out,
		std::size_t size);

	template<Endian endian>
	inline void convert_64(
		std::uint64_t const * in,
		std::uint64_t * out,
		std::size_t size);

	template<Endian endian>
	inline void convert_float(
		float const * in,
		float * out,
		std::size_t size);

	template<Endian endian>
	inline void convert_double(
		double const * in,
		double * out,
		std::size_t size);

	template<Endian endian, class T>
	inline void convert(
		T const * in,
		T * out,
		std::size_t size);

	template<Endian endian>
	inline void convert_16_par(
		std::uint16_t const * in,
		std::uint16_t * out,
		std::size_t size,
		std::size_t chunk_size = 0);

	template<Endian endian>
	inline void convert_32_par(
		std::uint32_t const * in,
		std::uint32_t * out,
		std::size_t size,
		std::size_t chunk_size = 0);

	template<Endian endian>
	inline void convert_64_par(
		std::uint64_t const * in,
		std::uint64_t * out,
		std::size_t size,
		std::size_t chunk_size = 0);

	template<Endian endian>
	inline void convert_float_par(
		float const * in,
		float * out,
		std::size_t size,
		std::size_t chunk_size = 0);

	template<Endian endian>
	inline void convert_double_par(
		double const * in,
		double * out,
		std::size_t size,
		std::size_t chunk_size = 0);

	template<Endian endian, class T>
	inline void convert_par(
		T const * in,
		T * out,
		std::size_t size,
		std::size_t chunk_size);
}

#include "ByteOrder.inl"

#undef ENDIANCVT

#endif