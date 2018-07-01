#ifndef __crystal_util_serialiser_hpp_defined
#define __crystal_util_serialiser_hpp_defined

#include "ByteOrder.hpp"

namespace crystal::util
{
	/** Serialises data. */
	class Serialiser
	{
		/** The target byte order. */
		ByteOrder m_byte_order;
	public:
		/** Creates a serialiser for a target byte order.
		@param[in] byte_order:
			The target byte order. */
		Serialiser(
			ByteOrder const& byte_order);

		/** Passes serialised data for further processing.
		@param[in] data:
			The data to serialise.
		@param[in] size:
			The byte size. */
		virtual void write(
			void const * data,
			std::size_t size) const = 0;

		inline Serialiser &operator<<(
			std::uint8_t v);
		inline Serialiser &operator<<(
			std::uint16_t v);
		inline Serialiser &operator<<(
			std::uint32_t v);
		inline Serialiser &operator<<(
			std::uint64_t v);
		inline Serialiser &operator<<(
			float v);
		inline Serialiser &operator<<(
			double v);
	};

	class Deserialiser
	{
		ByteOrder m_byte_order;
	public:
		Deserialiser(
			ByteOrder const& byte_order);

		virtual void read(
			void * data,
			std::size_t size) const = 0;

		inline Deserialiser &operator>>(
			std::uint8_t &v);
		inline Deserialiser &operator>>(
			std::uint16_t &v);
		inline Deserialiser &operator>>(
			std::uint32_t &v);
		inline Deserialiser &operator>>(
			std::uint64_t &v);
		inline Deserialiser &operator>>(
			float &v);
		inline Deserialiser &operator>>(
			double &v);
	};
}

#include "Serialiser.inl"

#endif