#ifndef __crystal_util_deserialiser_hpp_defined
#define __crystal_util_deserialiser_hpp_defined

namespace crystal::util
{
	/** Deserialises data.
	@tparam kEndian:
		The deserialiser's endianness.
	@tparam T:
		The deriving class.
		Must implement a function `void deserialise(void *, std::size_t)`.
		Must implement a static function `T &base(Deserialiser<kEndian, T> &)`. */
	template<Endian kEndian, class T>
	class Deserialiser
	{
	public:
		inline Deserialiser<kEndian, T> &operator>>(
			std::uint8_t &v);
		inline Deserialiser<kEndian, T> &operator>>(
			std::uint16_t &v);
		inline Deserialiser<kEndian, T> &operator>>(
			std::uint32_t &v);
		inline Deserialiser<kEndian, T> &operator>>(
			std::uint64_t &v);
		inline Deserialiser<kEndian, T> &operator>>(
			float &v);
		inline Deserialiser<kEndian, T> &operator>>(
			double &v);
	};

	template<class T>
	class DeserialiserUnion:
		Deserialiser<Endian::kLittle, DeserialiserUnion<T>>,
		Deserialiser<Endian::kBig, DeserialiserUnion<T>>
	{
	public:
		DeserialiserUnion() = default;

		typedef Deserialiser<Endian::kLittle, DeserialiserUnion<T>> LittleDeserialiser;
		typedef Deserialiser<Endian::kBig, DeserialiserUnion<T>> BigDeserialiser;

		inline Endian endian() const;

		constexpr Deserialiser<Endian::kLittle, DeserialiserUnion<T>> &little_deserialiser();
		constexpr Deserialiser<Endian::kBig, DeserialiserUnion<T>> &big_deserialiser();

		static constexpr DeserialiserUnion<T> &base(
			Deserialiser<Endian::kLittle, DeserialiserUnion<T>> &serialiser);
		static constexpr DeserialiserUnion<T> &base(
			Deserialiser<Endian::kBig, DeserialiserUnion<T>> &serialiser);

		inline void deserialise(
			void * data,
			std::size_t size);
	};

	template<Endian kEndian, class T>
	inline Deserialiser<kEndian, T> &operator>>(
		Deserialiser<kEndian, T> &s,
		Endian &v);
}

#include "Deserialiser.inl"

#endif