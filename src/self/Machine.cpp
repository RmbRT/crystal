#include "Machine.hpp"

namespace crystal::self
{
	Machine::Machine():
		crystal::Machine(Location::kSelf)
	{
	}

	Machine::Machine(
		util::latency_t computation_seq,
		util::latency_t computation_par):
		crystal::Machine(
			Location::kSelf,
			0, // no communication to self.
			computation_seq,
			computation_par)
	{
	}

	bool Machine::check_latencies(
		std::uint32_t seed,
		std::size_t data,
		std::size_t seq,
		std::size_t par)
	{
		m_communication = 0;
		m_computation_seq = util::latency(crystal::Machine::benchmark_seq, seed, seq);
		m_computation_par = util::latency(crystal::Machine::benchmark_par, seed, par, seq);

		return true;
	}
}