#ifndef __crystal_outside_machine_hpp_defined
#define __crystal_outside_machine_hpp_defined

#include "../remote/Machine.hpp"

namespace crystal::outside
{
	/** An untrusted machine outside the local area network. */
	class Machine : public remote::Machine
	{
	public:
		virtual bool exchange_keys() = 0;
	};
}

#endif