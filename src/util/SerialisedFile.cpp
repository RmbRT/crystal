#include "SerialisedFile.hpp"

namespace crystal::util
{
	SerialisedFile::SerialisedFile(
		char const * file,
		OpenMode mode,
		Endian endian)
	{
		open(file, mode, endian);
	}

	bool SerialisedFile::ensure_exists(
		char const * file)
	{
		std::ofstream f;
		f.open(file, std::ios::app);
		return f.is_open();
	}

	bool SerialisedFile::exists(
		char const * file)
	{
		std::ifstream f;
		f.open(file);
		return f.is_open();
	}

	void SerialisedFile::seek(
		std::streampos index)
	{
		if(m_has_dynamic_endian)
			std::fstream::seekp(index + std::streampos(1));
		else
			std::fstream::seekp(index);
	}

	void SerialisedFile::seek(
		std::streamoff offset,
		std::ios_base::seekdir way)
	{
		if(m_has_dynamic_endian
		&& way == std::ios::beg)
			std::fstream::seekp(offset + std::streamoff(1), std::ios::beg);
		else
			std::fstream::seekp(offset, way);
	}

	void SerialisedFile::seek(
		std::ios_base::seekdir way)
	{
		if(m_has_dynamic_endian
		&& way == std::ios::beg)
			std::fstream::seekp(std::streamoff(1), std::ios::beg);
		else
			std::fstream::seekp(std::streamoff(0), way);
	}

	std::streampos SerialisedFile::tell()
	{
		if(m_has_dynamic_endian)
			return std::fstream::tellp() - std::streampos(1);
		else
			return std::fstream::tellp();
	}

	void SerialisedFile::open(
		char const * file,
		OpenMode mode,
		Endian endian)
	{
		if(is_open())
			close();

		if(mode & OpenMode::kCreate)
		{
			if(mode & OpenMode::kClear)
			{
				std::fstream::open(file, std::ios::out | std::ios::trunc);

				if(mode & OpenMode::kDynamicEndian)
				{
					write(&endian, sizeof(Endian));
				}
			} else
			{
				if(mode & OpenMode::kDynamicEndian)
				{
					if(!exists(file))
					{
						SerialisedFile f;
						((std::fstream&)f).open(file, std::ios::out | std::ios::trunc);
						f.write(&endian, sizeof(Endian));
					}
					std::fstream::open(file, std::ios::in | std::ios::out);
					read(&endian, sizeof(Endian));
				} else
				{
					ensure_exists(file);
					std::fstream::open(file, std::ios::in | std::ios::out);
				}
			}
		} else
		{
			if(mode & OpenMode::kClear)
			{
				if(exists(file))
				{
					std::fstream::open(file, std::ios::out | std::ios::trunc);

					if(mode & OpenMode::kDynamicEndian)
					{
						write(&endian, sizeof(Endian));
					}
				}
			} else
			{
				std::fstream::open(file, std::ios::in | std::ios::out);
				if(mode & OpenMode::kDynamicEndian)
				{
					read(&endian, sizeof(Endian));
				}
			}
		}

		m_endian = endian;
	}
}