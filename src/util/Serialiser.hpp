#ifndef __crystal_util_serialiser_hpp_defined
#define __crystal_util_serialiser_hpp_defined

#include "ByteOrder.hpp"

namespace crystal::util
{
	/** Serialises data.
	@tparam kEndian:
		The serialiser's endianness.
	@tparam T:
		The deriving class.
		Must implement a function `void serialise(void const *, std::size_t)`.
		Must implement a static function `T &base(Serialiser<kEndian, T> &)`. */
	template<Endian kEndian, class T>
	class Serialiser
	{
	public:
		inline Serialiser<kEndian, T> &operator<<(
			std::uint8_t v);
		inline Serialiser<kEndian, T> &operator<<(
			std::uint16_t v);
		inline Serialiser<kEndian, T> &operator<<(
			std::uint32_t v);
		inline Serialiser<kEndian, T> &operator<<(
			std::uint64_t v);
		inline Serialiser<kEndian, T> &operator<<(
			float v);
		inline Serialiser<kEndian, T> &operator<<(
			double v);
	};

	template<class T>
	class SerialiserUnion:
		Serialiser<Endian::kLittle, SerialiserUnion<T>>,
		Serialiser<Endian::kBig, SerialiserUnion<T>>
	{
	public:
		SerialiserUnion() = default;

		typedef Serialiser<Endian::kLittle, SerialiserUnion<T>> LittleSerialiser;
		typedef Serialiser<Endian::kBig, SerialiserUnion<T>> BigSerialiser;

		constexpr Serialiser<Endian::kLittle, SerialiserUnion<T>> &little_serialiser();
		constexpr Serialiser<Endian::kBig, SerialiserUnion<T>> &big_serialiser();

		static constexpr SerialiserUnion<T> &base(
			Serialiser<Endian::kLittle, SerialiserUnion<T>> &serialiser);
		static constexpr SerialiserUnion<T> &base(
			Serialiser<Endian::kBig, SerialiserUnion<T>> &serialiser);

		inline void serialise(
			void const * data,
			std::size_t size);
	};

	template<Endian kEndian, class T>
	inline Serialiser<kEndian, T> &operator<<(
		Serialiser<kEndian, T> &s,
		Endian v);
}

#include "Serialiser.inl"

#endif