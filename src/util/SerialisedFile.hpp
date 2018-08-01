#ifndef __crystal_util_serialisedfile_hpp_defined
#define __crystal_util_serialisedfile_hpp_defined

#include "Serialiser.hpp"
#include "Deserialiser.hpp"

#include <fstream>

namespace crystal::util
{
	/** File opening behaviour. */
	enum class OpenMode : std::uint8_t
	{
		/** Only opens existing files. */
		kPassive = 0,
		/** Creates nonexistent files. */
		kCreate = 1,
		/** Empties the file, if opened. */
		kClear = 2,
		/** The file contains its endianness at the beginning of the file. */
		kDynamicEndian = 4
	};

	constexpr OpenMode operator|(OpenMode, OpenMode);
	constexpr bool operator&(OpenMode, OpenMode);

	class SerialisedFile :
		protected std::fstream,
		private SerialiserUnion<SerialisedFile>,
		private DeserialiserUnion<SerialisedFile>
	{
		bool m_has_dynamic_endian;
		Endian m_endian;
	public:
		using std::fstream::is_open;
		using std::fstream::close;

		using std::fstream::gcount;
		using std::fstream::get;
		using std::fstream::getline;
		using std::fstream::ignore;
		using std::fstream::peek;
		using std::fstream::putback;
		using std::fstream::unget;
		using std::fstream::sync;

		using std::fstream::put;
		using std::fstream::flush;

		using std::fstream::good;
		using std::fstream::eof;
		using std::fstream::fail;
		using std::fstream::bad;
		using std::fstream::operator!;
		using std::fstream::operator bool;
		using std::fstream::rdstate;
		using std::fstream::setstate;
		using std::fstream::clear;
		using std::fstream::copyfmt;
		using std::fstream::fill;
		using std::fstream::exceptions;
		using std::fstream::imbue;
		using std::fstream::tie;
		using std::fstream::rdbuf;
		using std::fstream::narrow;
		using std::fstream::widen;

		using std::fstream::flags;
		using std::fstream::setf;
		using std::fstream::unsetf;
		using std::fstream::precision;
		using std::fstream::width;

		using std::fstream::getloc;
		using std::fstream::xalloc;
		using std::fstream::iword;
		using std::fstream::pword;
		using std::fstream::register_callback;
		using std::fstream::sync_with_stdio;

		SerialisedFile() = default;

		/** Opens a file with a predefined endianness.
		@param[in] file:
			The file to open.
		@param[in] mode:
			The desired behaviour for opening the file.
		@param[in] endian:
			The file's endianness. */
		SerialisedFile(
			char const * file,
			OpenMode mode = OpenMode::kPassive,
			Endian endian = endian::kSelf);

		/** Makes sure the file exists.
		@param[in] file:
			The file whose existence to ensure.
		@return
			True if the file exists, false if creation failed. */
		static bool ensure_exists(
			char const * file);

		/** Checks whether a file exists.
		@param[in] file:
			The file to check for.
		@return
			Whether the file exists (also fails if it is already opened). */
		static bool exists(
			char const * file);

		/** Seeks to the given index.
		@param[in] index:
			The byte index to seek to. */
		void seek(
			std::streampos index);

		/** Seeks to the given position.
		@param[in] offset:
			The offset from `way`.
		@param[in] way:
			The position to seek from. */
		void seek(
			std::streamoff offset,
			std::ios_base::seekdir way);

		/** Seeks to the given position.
		@param[in] way:
			The position to seek to. */
		void seek(
			std::ios_base::seekdir way);

		/** Returns the byte index in the file. */
		std::streampos tell();

		template<class T>
		/** Serialises data.
		@param[in] value:
			The data to serialise.
		@return
			A reference to the file. */
		inline SerialisedFile &operator<<(
			T const& value);

		template<class T>
		/** Deserialises data.
		@param[in] value:
			The data to deserialise into.
		@return
			A reference to the file. */
		inline SerialisedFile &operator>>(
			T &value);

		template<class T>
		/** Calls `std::fstream::operator<<`.
		@param[in] v:
			The value to be written. */
		inline void write_formatted(
			T const& v);

		template<class T>
		/** Calls `std::fstream::operator>>`.
		@param[out] v:
			The value to be read. */
		inline void read_formatted(
			T &v);

		/** Writes raw data to the file.
		@param[in] data:
			The data to write.
		@param[in] size:
			The byte size. */
		inline void write(
			void const * data,
			std::size_t size);

		/** Reads raw data from the file.
		@param[out] data:
			The data to read.
		@param[in] size:
			The byte size. */
		inline void read(
			void * data,
			std::size_t size);

		/** Opens a file with a predefined endianness.
		@param[in] file:
			The file to open.
		@param[in] mode:
			The desired behaviour for opening the file.
		@param[in] endian:
			The file's endianness.
			If the file already exists and contains endianness, the file's endianness is used instead. */
		void open(
			char const * file,
			OpenMode mode = OpenMode::kPassive,
			Endian endian = endian::kSelf);

		// Serialiser and Deserialiser.
		using SerialiserUnion<SerialisedFile>::LittleSerialiser;
		using SerialiserUnion<SerialisedFile>::BigSerialiser;
		using DeserialiserUnion<SerialisedFile>::LittleDeserialiser;
		using DeserialiserUnion<SerialisedFile>::BigDeserialiser;

		inline LittleSerialiser &little_serialiser();
		inline BigSerialiser &big_serialiser();

		inline LittleDeserialiser &little_deserialiser();
		inline BigDeserialiser &big_deserialiser();

		static constexpr SerialisedFile &base(
			SerialiserUnion<SerialisedFile> &serialiser);
		static constexpr SerialisedFile &base(
			DeserialiserUnion<SerialisedFile> &deserialiser);

		inline void serialise(
			void const * data,
			std::size_t size);
		inline void deserialise(
			void * data,
			std::size_t size);
	};
}

#include "SerialisedFile.inl"

#endif