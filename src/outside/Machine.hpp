#ifndef __crystal_outside_machine_hpp_defined
#define __crystal_outside_machine_hpp_defined

#include "../remote/Machine.hpp"

namespace crystal::outside
{
	/** An untrusted machine outside the local area network. */
	template<util::Endian kEndian>
	class Machine : public remote::Machine<kEndian>
	{
	public:
		virtual bool exchange_keys() = 0;
	};
}

#endif