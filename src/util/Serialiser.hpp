#ifndef __crystal_util_serialiser_hpp_defined
#define __crystal_util_serialiser_hpp_defined

#include "ByteOrder.hpp"

namespace crystal::util
{
	/** Serialises data. */
	class Serialiser
	{
	protected:
		/** The target byte order. */
		ByteOrder m_byte_order;
		/** Whether the write stream still works. */
		bool m_good;
	public:
		/** Creates a serialiser for a target byte order.
		@param[in] byte_order:
			The target byte order. */
		Serialiser(
			ByteOrder const& byte_order);

		/** Returns whether the write stream had any errors. */
		inline operator bool() const;

		/** Sets the target byte order.
		@param[in] byte_order:
			The target byte order. */
		void set_target_order(
			ByteOrder const& byte_order);

		/** Passes serialised data for further processing.
		@param[in] data:
			The data to serialise.
		@param[in] size:
			The byte size.
		@return
			The serialiser. */
		virtual Serialiser &write(
			void const * data,
			std::size_t size) = 0;

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

	/** Deserialises data. */
	class Deserialiser
	{
	protected:
		/** The source byte order. */
		ByteOrder m_byte_order;
		bool m_good;
	public:
		/** Creates a deserialiser with the given source byte order.
		@param[in] byte_order:
			The source byte order. */
		Deserialiser(
			ByteOrder const& byte_order);

		/** Returns whether the read stream had any errors. */
		inline operator bool() const;

		/** Sets the source byte order.
		@param[in] order:
			The source byte order. */
		void set_source_order(
			ByteOrder const& byte_order);

		/** Reads serialised data for deserialisation.
		@param[in] data:
			The buffer to read into.
		@param[in] size:
			The byte size.
		@return
			The deserialiser. */
		virtual Deserialiser &read(
			void * data,
			std::size_t size) = 0;

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