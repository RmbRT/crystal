#ifndef __crystal_local_machine_hpp_defined
#define __crystal_local_machine_hpp_defined

#include "../Machine.hpp"

namespace crystal::local
{
	/** Representation of the local machine. */
	class Machine : public crystal::Machine
	{
	public:
		/** Creates an uninitialised local machine. */
		Machine();

		/** Creates a local machine with the given communication latency.
		@param[in] computation_seq:
			The sequential computation latency.
		@param[in] computation_par:
			The parallel computation latency. */
		Machine(
			util::latency_t computation_seq,
			util::latency_t computation_par);

		bool check_latencies(
			std::uint32_t seed,
			std::size_t data,
			std::size_t seq,
			std::size_t par) override;
	};
}

#endif