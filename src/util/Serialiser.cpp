#include "Serialiser.hpp"
#include <vector>
namespace crystal::util
{
	Serialiser::Serialiser(
		ByteOrder const& byte_order):
		m_byte_order(byte_order),
		m_good(true)
	{
	}

	Deserialiser::Deserialiser(
		ByteOrder const& byte_order):
		m_byte_order(byte_order),
		m_good(true)
	{
	}
}