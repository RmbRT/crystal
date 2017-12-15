#ifndef __crystal_self_machine_hpp_defined
#define __crystal_self_machine_hpp_defined

#include "../Machine.hpp"

namespace crystal::self
{
	/** Representation of the host machine. */
	class Machine : public crystal::Machine
	{
	public:
		/** Creates an uninitialised host machine. */
		Machine();

		/** Creates a host machine with the given communication latency.
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

		/** The host machine's byte order. */
		static constexpr std::uint64_t byte_order();
	};
}

#endif