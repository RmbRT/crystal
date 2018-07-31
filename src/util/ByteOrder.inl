#include <algorithm>
#include "../self/Paralleliser.hpp"

namespace crystal::util::endian
{
	constexpr std::uint16_t reverse_16(
		std::uint16_t v)
	{
		return (v << 8) | (v >> 8);
	}

	constexpr std::uint32_t reverse_32(
		std::uint32_t v)
	{
		return reverse_16(v<<16) | reverse_16(v>>16);
	}

	constexpr std::uint64_t reverse_64(
		std::uint64_t v)
	{
		return reverse_32(v<<32) | reverse_32(v>>32);
	}

	namespace detail
	{
		constexpr float to_float(
			std::uint32_t u)
		{
			return reinterpret_cast<float const&>(u);
		}
		constexpr double to_double(
			std::uint64_t u)
		{
			return reinterpret_cast<double const&>(u);
		}
	}

	constexpr float reverse_float(
		float v)
	{
		return detail::to_float(
			reverse_32(
				reinterpret_cast<std::uint32_t const&>(v)));
	}

	constexpr float reverse_double(
		double v)
	{
		return detail::to_double(
			reverse_64(
				reinterpret_cast<std::uint64_t const&>(v)));
	}

	template<>
	constexpr std::uint16_t reverse<std::uint16_t>(
		std::uint16_t v)
	{
		return reverse_16(v);
	}

	template<>
	constexpr std::uint32_t reverse<std::uint32_t>(
		std::uint32_t v)
	{
		return reverse_32(v);
	}

	template<>
	constexpr std::uint64_t reverse<std::uint64_t>(
		std::uint64_t v)
	{
		return reverse_64(v);
	}

	template<>
	constexpr float reverse<float>(
		float v)
	{
		return reverse_float(v);
	}

	template<>
	constexpr double reverse<double>(
		double v)
	{
		return reverse_double(v);
	}

	template<Endian endian>
	ENDIANCVT std::uint16_t convert_16(
		std::uint16_t v)
	{
		if(endian == kSelf)
			return v;
		else
			return reverse_16(v);
	}

	template<Endian endian>
	ENDIANCVT std::uint32_t convert_32(
		std::uint32_t v)
	{
		if(endian == kSelf)
			return v;
		else
			return reverse_32(v);
	}

	template<Endian endian>
	ENDIANCVT std::uint64_t convert_64(
		std::uint64_t v)
	{
		if(endian == kSelf)
			return v;
		else
			return reverse_64(v);
	}

	template<Endian endian>
	ENDIANCVT float convert_float(
		float v)
	{
		if(endian == kSelf)
			return v;
		else
			return reverse_float(v);
	}

	template<Endian endian>
	ENDIANCVT double convert_double(
		double v)
	{
		if(endian == kSelf)
			return v;
		else
			return reverse_double(v);
	}

	template<Endian endian, class T>
	inline T convert(
		T v)
	{
		if(endian == kSelf)
			return v;
		else
			return reverse<T>(v);
	}

	template<Endian endian>
	void convert_16(
		std::uint16_t const * in,
		std::uint16_t * out,
		std::size_t size)
	{
		if(endian != kSelf)
			for(std::size_t i = 0; i < size; i++)
				out[i] = reverse_16(in[i]);
		else if(in != out)
			std::copy(in, in+size, out);
	}

	template<Endian endian>
	void convert_32(
		std::uint32_t const * in,
		std::uint32_t * out,
		std::size_t size)
	{
		if(endian != kSelf)
			for(std::size_t i = 0; i < size; i++)
				out[i] = reverse_32(in[i]);
		else if(in != out)
			std::copy(in, in+size, out);
	}

	template<Endian endian>
	void convert_64(
		std::uint64_t const * in,
		std::uint64_t * out,
		std::size_t size)
	{
		if(endian != kSelf)
			for(std::size_t i = 0; i < size; i++)
				out[i] = reverse_64(in[i]);
		else if(in != out)
			std::copy(in, in+size, out);
	}

	template<Endian endian>
	void convert_float(
		float const * in,
		float * out,
		std::size_t size)
	{
		if(endian != kSelf)
			for(std::size_t i = 0; i < size; i++)
				out[i] = reverse_float(in[i]);
		else if(in != out)
			std::copy(in, in+size, out);
	}

	template<Endian endian>
	void convert_double(
		double const * in,
		double * out,
		std::size_t size)
	{
		if(endian != kSelf)
			for(std::size_t i = 0; i < size; i++)
				out[i] = reverse_double(in[i]);
		else if(in != out)
			std::copy(in, in+size, out);
	}

	template<Endian endian, class T>
	void convert(
		T const * in,
		T * out,
		std::size_t size)
	{
		if(endian != kSelf)
			for(std::size_t i = 0; i < size; i++)
				out[i] = reverse<T>(in[i]);
		else if(in != out)
			std::copy(in, in+size, out);
	}

	template<Endian endian>
	void convert_16_par(
		std::uint16_t const * in,
		std::uint16_t * out,
		std::size_t size,
		std::size_t chunk_size)
	{
		self::Paralleliser::loop_chunks(
			0,
			size,
			chunk_size,
			[](
				std::size_t begin,
				std::size_t count,
				std::uint16_t const * in,
				std::uint16_t * out)
			{
				convert_16<endian>(
					in+begin,
					out+begin,
					count);
			},
			in,
			out);
	}

	template<Endian endian>
	void convert_32_par(
		std::uint32_t const * in,
		std::uint32_t * out,
		std::size_t size,
		std::size_t chunk_size)
	{
		self::Paralleliser::loop_chunks(
			0,
			size,
			chunk_size,
			[](
				std::size_t begin,
				std::size_t count,
				std::uint32_t const * in,
				std::uint32_t * out)
			{
				convert_32<endian>(
					in+begin,
					out+begin,
					count);
			},
			in,
			out);
	}

	template<Endian endian>
	void convert_64_par(
		std::uint64_t const * in,
		std::uint64_t * out,
		std::size_t size,
		std::size_t chunk_size)
	{
		self::Paralleliser::loop_chunks(
			0,
			size,
			chunk_size,
			[](
				std::size_t begin,
				std::size_t count,
				std::uint64_t const * in,
				std::uint64_t * out)
			{
				convert_64<endian>(
					in+begin,
					out+begin,
					count);
			},
			in,
			out);
	}

	template<Endian endian>
	void convert_float_par(
		float const * in,
		float * out,
		std::size_t size,
		std::size_t chunk_size)
	{
		self::Paralleliser::loop_chunks(
			0,
			size,
			chunk_size,
			[](
				std::size_t begin,
				std::size_t count,
				float const * in,
				float * out)
			{
				convert_float<endian>(
					in+begin,
					out+begin,
					count);
			},
			in,
			out);
	}

	template<Endian endian>
	void convert_double_par(
		double const * in,
		double * out,
		std::size_t size,
		std::size_t chunk_size)
	{
		self::Paralleliser::loop_chunks(
			0,
			size,
			chunk_size,
			[](
				std::size_t begin,
				std::size_t count,
				double const * in,
				double * out)
			{
				convert_double<endian>(
					in+begin,
					out+begin,
					count);
			},
			in,
			out);
	}

	template<Endian endian, class T>
	void convert_par(
		T const * in,
		T * out,
		std::size_t size,
		std::size_t chunk_size)
	{
		self::Paralleliser::loop_chunks(
			0,
			size,
			chunk_size,
			[](
				std::size_t begin,
				std::size_t count,
				T const * in,
				T * out)
			{
				convert<endian, T>(
					in+begin,
					out+begin,
					count);
			},
			in,
			out);
	}
}