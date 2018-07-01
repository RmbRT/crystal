#include "Serialiser.hpp"
#include <vector>
namespace crystal::util
{
	Serialiser::Serialiser(
		ByteOrder const& byte_order):
		m_byte_order(byte_order)
	{
	}

	Deserialiser::Deserialiser(
		ByteOrder const& byte_order):
		m_byte_order(byte_order)
	{
	}
}